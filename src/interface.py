import os
import sys
import locale
import subprocess
from PyQt6 import QtCore, QtWidgets
from PyQt6 import QtGui
import shutil

class Worker(QtCore.QThread):
    output = QtCore.pyqtSignal(str)
    finished = QtCore.pyqtSignal(str)
    error = QtCore.pyqtSignal(str)

    def __init__(self, command, output_dir):
        super().__init__()
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
                self.error.emit(f"Error: process finished with code {process.returncode}")
        except Exception as e:
            self.error.emit(str(e))

def limpiar_carpeta(path):
    if os.path.exists(path) and os.path.isdir(path):
        for nombre in os.listdir(path):
            ruta = os.path.join(path, nombre)
            try:
                if os.path.isfile(ruta) or os.path.islink(ruta):
                    os.unlink(ruta)
                elif os.path.isdir(ruta):
                    shutil.rmtree(ruta)
            except Exception as e:
                print(f"Error deleting {ruta}: {e}")

def count_processed_images(folders):
    count = 0
    for folder in folders:
        try:
            count += len([f for f in os.listdir(folder) if f.lower().endswith('.bmp')])
        except FileNotFoundError:
            pass
    return count

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(390, 340)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Policy.Fixed, QtWidgets.QSizePolicy.Policy.Fixed)
        Dialog.setSizePolicy(sizePolicy)
        Dialog.setMinimumSize(QtCore.QSize(390, 340))
        Dialog.setMaximumSize(QtCore.QSize(390, 340))

        self.menubar = QtWidgets.QMenuBar(Dialog)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 390, 21))
        self.menuEquipo = QtWidgets.QMenu("Team", self.menubar)
        self.menubar.addMenu(self.menuEquipo)
        self.actionMostrarMiembros = QtGui.QAction("Team members", Dialog)
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
        self.progressBar.setFormat("Processing: %p%")

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

    def obtener_total_slots(self, machinefile_path="machinefile"):
        total_slots = 0
        try:
            with open(machinefile_path, "r") as f:
                for line in f:
                    if "slots=" in line:
                        partes = line.strip().split("slots=")
                        if len(partes) == 2:
                            try:
                                total_slots += int(partes[1])
                            except ValueError:
                                continue
        except FileNotFoundError:
            QtWidgets.QMessageBox.warning(None, "Warning", "Machinefile not found.")
        return total_slots if total_slots > 0 else 1  # minimum 1 process

    def open_file_dialog(self):
        dir_name = QtWidgets.QFileDialog.getExistingDirectory(None, "Select folder", "")

        if dir_name:
            self.fileLineEdit.setText(dir_name)

    def start_processing(self):
                # Obtener base_dir correcto (padre de la carpeta seleccionada)
        base_dir = os.path.dirname(self.fileLineEdit.text())

        # Definir carpetas de resultado para limpiar
        folders_to_clean = [
            os.path.join(base_dir, "result", "Gray"),
            os.path.join(base_dir, "result", "HorizontalGray"),
            os.path.join(base_dir, "result", "HorizontalColor"),
            os.path.join(base_dir, "result", "VerticalGray"),
            os.path.join(base_dir, "result", "VerticalColor"),
            os.path.join(base_dir, "result", "Blur"),
        ]

        # Limpiar las carpetas antes de procesar
        for folder in folders_to_clean:
            limpiar_carpeta(folder)
        input_dir = self.fileLineEdit.text()
        blur_ratio = str(self.blurSlider.value())
        output_dir = os.path.join(input_dir, "result")
        num_filters = 6
        countOriginal = 0
        countOriginal += len([f for f in os.listdir(input_dir) if f.lower().endswith('.bmp')])
        print(countOriginal)
        total_images = countOriginal
        self.total_expected = total_images * num_filters
        total_slots = self.obtener_total_slots()
        print(total_slots)
        command = [
            "mpirun", "-np", str(total_slots), "--hostfile", "machinefile", "./run.sh", blur_ratio, input_dir
        ]

        self.outputTextEdit.clear()
        self.outputTextEdit.append("Processing...")

        self.worker = Worker(command, output_dir)
        self.worker.output.connect(self.append_output)
        self.worker.finished.connect(self.on_finished)
        self.worker.error.connect(self.on_error)
        self.worker.start()

        self.progressTimer = QtCore.QTimer()
        self.progressTimer.timeout.connect(self.update_progress_from_files)
        self.progressTimer.start(500)

    def update_progress_from_files(self):
        base_dir = os.path.dirname(self.fileLineEdit.text())
        folders = [
            os.path.join(base_dir, "result", "Gray"),
            os.path.join(base_dir, "result", "HorizontalGray"),
            os.path.join(base_dir, "result", "HorizontalColor"),
            os.path.join(base_dir, "result", "VerticalGray"),
            os.path.join(base_dir, "result", "VerticalColor"),
            os.path.join(base_dir, "result", "Blur"),
        ]
        processed = count_processed_images(folders)
        porcentaje = int((processed / self.total_expected) * 100)
        self.progressBar.setValue(min(porcentaje, 100))

    def append_output(self, text):
        self.outputTextEdit.append(text)
        self.outputTextEdit.verticalScrollBar().setValue(self.outputTextEdit.verticalScrollBar().maximum())

    def on_finished(self, output_dir):
        if hasattr(self, 'progressTimer'):
            self.progressTimer.stop()
        self.progressBar.setValue(100)
        self.outputTextEdit.append(f"\nFiles saved in:\n{output_dir}")
        self.mostrar_reporte_global()

    def mostrar_reporte_global(self):
        ruta_reporte = os.path.join(self.fileLineEdit.text(), "report_global.txt")
        if os.path.exists(ruta_reporte):
            self.outputTextEdit.append("\n--- PERFORMANCE REPORT ---")
            with open(ruta_reporte, "r") as f:
                for line in f:
                    self.outputTextEdit.append(line.strip())

    def on_error(self, message):
        if hasattr(self, 'progressTimer'):
            self.progressTimer.stop()
        self.outputTextEdit.append(f"\nError during processing:\n{message}")
        QtWidgets.QMessageBox.critical(None, "Error", f"Failed to execute processing:\n{message}")
        self.progressBar.setValue(0)

    def mostrar_miembros_equipo(self):
        QtWidgets.QMessageBox.information(
            None,
            "Miembros del equipo",
            "Christian Flores\nSarai Santiago\nMarlon Yahir\nAlejandro Moctezuma"
        )

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.exec()