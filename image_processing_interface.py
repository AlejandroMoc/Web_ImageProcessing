import os
import sys
import locale
import subprocess
from PyQt6 import QtCore, QtWidgets
from PyQt6 import QtGui

class Worker(QtCore.QThread):
    output = QtCore.pyqtSignal(str)
    finished = QtCore.pyqtSignal(str)
    error = QtCore.pyqtSignal(str)

    def _init_(self, command, output_dir):
        super()._init_()
        self.command = command
        self.output_dir = output_dir

    def run(self):
        try:
            process = subprocess.Popen(
                self.command,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True
            )
            for line in process.stdout:
                self.output.emit(line.rstrip())
            process.wait()
            if process.returncode == 0:
                self.finished.emit(self.output_dir)
            else:
                self.error.emit(f"Error: proceso terminó con código {process.returncode}")
        except Exception as e:
            self.error.emit(str(e))

def count_processed_images(folders):
    count = 0
    for folder in folders:
        try:
            count += len([f for f in os.listdir(folder) if f.lower().endswith('.bmp')])
            print(count)
        except FileNotFoundError:
            pass  # carpeta aún no creada
    return count

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(390, 340)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Policy.Fixed, QtWidgets.QSizePolicy.Policy.Fixed)
        Dialog.setSizePolicy(sizePolicy)
        Dialog.setMinimumSize(QtCore.QSize(390, 340))
        Dialog.setMaximumSize(QtCore.QSize(390, 340))

        # Barra de menú
        self.menubar = QtWidgets.QMenuBar(Dialog)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 390, 21))
        self.menuEquipo = QtWidgets.QMenu("Equipo", self.menubar)
        self.menubar.addMenu(self.menuEquipo)
        self.actionMostrarMiembros = QtGui.QAction("Miembros del equipo", Dialog)
        self.actionMostrarMiembros.triggered.connect(self.mostrar_miembros_equipo)
        self.menuEquipo.addAction(self.actionMostrarMiembros)
        Dialog.layout = QtWidgets.QVBoxLayout(Dialog)
        Dialog.layout.setMenuBar(self.menubar)

        self.buttonBox = QtWidgets.QDialogButtonBox(parent=Dialog)
        self.buttonBox.setGeometry(QtCore.QRect(20, 300, 350, 30))
        self.buttonBox.setOrientation(QtCore.Qt.Orientation.Horizontal)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.StandardButton.Cancel | QtWidgets.QDialogButtonBox.StandardButton.Ok)

        self.label_image = QtWidgets.QLabel(parent=Dialog)
        self.label_image.setGeometry(QtCore.QRect(20, 30, 350, 20))

        self.fileLineEdit = QtWidgets.QLineEdit(parent=Dialog)
        self.fileLineEdit.setGeometry(QtCore.QRect(20, 60, 300, 30))

        self.browseButton = QtWidgets.QPushButton("...", parent=Dialog)
        self.browseButton.setGeometry(QtCore.QRect(330, 60, 40, 30))
        self.browseButton.clicked.connect(self.open_file_dialog)

        self.label_filter = QtWidgets.QLabel(parent=Dialog)
        self.label_filter.setGeometry(QtCore.QRect(20, 130, 170, 30))

        self.comboBox = QtWidgets.QComboBox(parent=Dialog)
        self.comboBox.setGeometry(QtCore.QRect(210, 130, 160, 30))

        self.label_kernel = QtWidgets.QLabel(parent=Dialog)
        self.label_kernel.setGeometry(QtCore.QRect(20, 170, 200, 30))

        self.blurSlider = QtWidgets.QSlider(QtCore.Qt.Orientation.Horizontal, parent=Dialog)
        self.blurSlider.setGeometry(QtCore.QRect(210, 170, 100, 30))
        self.blurSlider.setRange(55, 155)
        self.blurSlider.setSingleStep(2)
        self.blurSlider.setValue(55)

        self.blurValueLabel = QtWidgets.QLabel("55", parent=Dialog)
        self.blurValueLabel.setGeometry(QtCore.QRect(320, 170, 40, 30))

        self.blurSlider.valueChanged.connect(lambda value: self.blurValueLabel.setText(str(value)))

        self.progressBar = QtWidgets.QProgressBar(parent=Dialog)
        self.progressBar.setGeometry(QtCore.QRect(20, 270, 350, 20))
        self.progressBar.setMinimum(0)
        self.progressBar.setMaximum(100)
        self.progressBar.setValue(0)
        self.progressBar.setFormat("Procesamiento: %p%")

        self.outputTextEdit = QtWidgets.QTextEdit(parent=Dialog)
        self.outputTextEdit.setGeometry(QtCore.QRect(20, 95, 350, 70))
        self.outputTextEdit.setReadOnly(True)
        self.outputTextEdit.setStyleSheet("color: #AAAAAA; font-family: monospace; font-size: 11px;")

        self.retranslateUi(Dialog)
        self.buttonBox.accepted.connect(self.start_processing)
        self.buttonBox.rejected.connect(Dialog.reject)
        QtCore.QMetaObject.connectSlotsByName(Dialog)
        self.comboBox.currentIndexChanged.connect(self.updateKernelVisibility)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        lang, _ = locale.getlocale()
        if lang == "es" or lang == "es_MX":
            Dialog.setWindowTitle(_translate("Dialog", "Procesamiento de imágenes"))
            self.label_image.setText(_translate("Dialog", "Selecciona la carpeta a procesar"))
            self.label_filter.setText(_translate("Dialog", "Selecciona el tipo de filtro"))
            self.label_kernel.setText(_translate("Dialog", "Selecciona el valor del núcleo"))
            self.browseButton.setText(_translate("Dialog", "..."))
            self.comboBox.clear()
            self.comboBox.addItems([
                "Todo", "Original", "Desenfoque", "Gris",
                "Horizontal a BYN", "Horizontal a Color",
                "Vertical a BYN", "Vertical a Color"
            ])
        else:
            Dialog.setWindowTitle(_translate("Dialog", "Image Processing"))
            self.label_image.setText(_translate("Dialog", "Select the folder to process"))
            self.label_filter.setText(_translate("Dialog", "Select the filter type"))
            self.label_kernel.setText(_translate("Dialog", "Select the kernel value"))
            self.browseButton.setText(_translate("Dialog", "..."))
            self.comboBox.clear()
            self.comboBox.addItems([
                "All", "Original", "Blurred", "Gray",
                "Horizontal Gray", "Horizontal Color",
                "Vertical Gray", "Vertical Color"
            ])
        self.updateKernelVisibility()

    def updateKernelVisibility(self):
        filtro = self.comboBox.currentText()
        if filtro in ["Todo", "All", "Desenfoque", "Blurred"]:
            self.label_kernel.show()
            self.blurSlider.show()
            self.blurValueLabel.show()
        else:
            self.label_kernel.hide()
            self.blurSlider.hide()
            self.blurValueLabel.hide()

    def open_file_dialog(self):
        dir_name = QtWidgets.QFileDialog.getExistingDirectory(None, "Selecciona la carpeta", "")
        if dir_name:
            self.fileLineEdit.setText(dir_name)

    def start_processing(self):
        input_dir = self.fileLineEdit.text()
        blur_ratio = str(self.blurSlider.value())
        output_dir = os.path.join(input_dir, "Result")

        # Suponiendo que sabes cuántos filtros produces (6 en este ejemplo)
        num_filters = 6

        # Aquí deberías calcular total_images, por ejemplo leyendo carpeta input_dir
        # Para demo, ponemos 1000, cambia esto por conteo real
        total_images = 10  
        self.total_expected = total_images * num_filters

        command = [
            "mpirun", "-np", "3", "--hostfile", "machinefile", "./run.sh", blur_ratio, input_dir
        ]

        self.outputTextEdit.clear()
        self.outputTextEdit.append("Procesando...")

        self.worker = Worker(command, output_dir)
        self.worker.output.connect(self.append_output)
        self.worker.finished.connect(self.on_finished)
        self.worker.error.connect(self.on_error)
        self.worker.start()

        # Timer para actualizar barra de progreso
        self.progressTimer = QtCore.QTimer()
        self.progressTimer.timeout.connect(self.update_progress_from_files)
        self.progressTimer.start(500)  # cada 0.5 segundos

    def update_progress_from_files(self):
        
        base_dir = os.path.dirname(self.fileLineEdit.text())
        print(base_dir)
        folders = [
            os.path.join(base_dir, "Result", "Gray"),
            os.path.join(base_dir, "Result", "HorizontalGray"),
            os.path.join(base_dir, "Result", "HorizontalColor"),
            os.path.join(base_dir, "Result", "VerticalGray"),
            os.path.join(base_dir, "Result", "VerticalColor"),
            os.path.join(base_dir, "Result", "Blur"),
        ]
        processed = count_processed_images(folders)
        porcentaje = int((processed / self.total_expected) * 100)
        if porcentaje > 100:
            porcentaje = 100
        self.progressBar.setValue(porcentaje)

    def append_output(self, text):
        self.outputTextEdit.append(text)
        self.outputTextEdit.verticalScrollBar().setValue(self.outputTextEdit.verticalScrollBar().maximum())

    def on_finished(self, output_dir):
        if hasattr(self, 'progressTimer'):
            self.progressTimer.stop()
        self.progressBar.setValue(100)
        self.outputTextEdit.append(f"\nArchivos guardados en:\n{output_dir}")

    def on_error(self, message):
        if hasattr(self, 'progressTimer'):
            self.progressTimer.stop()
        self.outputTextEdit.append(f"\nError durante el procesamiento:\n{message}")
        QtWidgets.QMessageBox.critical(None, "Error", f"Fallo al ejecutar el procesamiento:\n{message}")
        self.progressBar.setValue(0)

    def mostrar_miembros_equipo(self):
        QtWidgets.QMessageBox.information(
            None,
            "Miembros del equipo",
            "Christian Flores - A01734997\nSarai Santiago - A01735331\nMarlon Yahir - A01424875\nAlejandro Moctezuma - A01736353"
        )

if _name_ == "_main_":
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.exec()