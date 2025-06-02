import os, sys, ctypes, locale
from PyQt6 import QtCore, QtWidgets

#Cargar la biblioteca compartida
image_processing = ctypes.CDLL('./image_processing.so')

#Definir los argumentos y el tipo de retorno de las funciones
image_processing.process_images_mirror_horizontal_gray.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
image_processing.process_images_mirror_horizontal_gray.restype = None
image_processing.processing_all.argtypes = [ctypes.c_char_p, ctypes.c_int]
image_processing.processing_all.restype = None

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(390, 300)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Policy.Fixed, QtWidgets.QSizePolicy.Policy.Fixed)
        Dialog.setSizePolicy(sizePolicy)
        Dialog.setMinimumSize(QtCore.QSize(390, 300))
        Dialog.setMaximumSize(QtCore.QSize(390, 300))

        self.buttonBox = QtWidgets.QDialogButtonBox(parent=Dialog)
        self.buttonBox.setGeometry(QtCore.QRect(20, 230, 350, 30))
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

        self.spinBox = QtWidgets.QSpinBox(parent=Dialog)
        self.spinBox.setGeometry(QtCore.QRect(300, 180, 70, 30))
        self.spinBox.setRange(55, 155)
        self.spinBox.setSingleStep(2)

        self.label_kernel = QtWidgets.QLabel(parent=Dialog)
        self.label_kernel.setGeometry(QtCore.QRect(20, 180, 170, 30))

        self.retranslateUi(Dialog)
        self.buttonBox.accepted.connect(self.on_accept)
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
        if self.comboBox.currentText() in ["Todo", "All", "Desenfoque", "Blurred"]:
            self.label_kernel.show()
            self.spinBox.show()
        else:
            self.label_kernel.hide()
            self.spinBox.hide()

    def open_file_dialog(self):
        dir_name = QtWidgets.QFileDialog.getExistingDirectory(None, "Selecciona la carpeta", "")
        if dir_name:
            self.fileLineEdit.setText(dir_name)

    def on_accept(self):
        filtro_seleccionado = self.comboBox.currentText()
        input_dir = self.fileLineEdit.text()
        print(f"Filtro seleccionado: '{filtro_seleccionado}'")

        # Mapa de traducciones
        mapa_filtros = {
            "Todo": "all", "All": "all",
            "Desenfoque": "blur", "Blurred": "blur",
            "Gris": "gray", "Gray": "gray",
            "Horizontal a BYN": "hgray", "Horizontal Gray": "hgray",
            "Horizontal a Color": "hcolor", "Horizontal Color": "hcolor",
            "Vertical a BYN": "vgray", "Vertical Gray": "vgray",
            "Vertical a Color": "vcolor", "Vertical Color": "vcolor"
        }

        filtro = mapa_filtros.get(filtro_seleccionado, None)

        if filtro == "all":
            print("Procesando todos los filtros")
            blur_ratio = self.spinBox.value()
            image_processing.processing_all(input_dir.encode('utf-8'), blur_ratio)
        elif filtro == "blur":
            print("Procesando desenfoque")
            blur_ratio = self.spinBox.value()
            image_processing.process_images_blur_color(input_dir.encode('utf-8'), os.path.join(input_dir, "Blur").encode('utf-8'))
        elif filtro == "gray":
            print("Procesando gris")
            image_processing.process_images_gray(input_dir.encode('utf-8'), os.path.join(input_dir, "Gray").encode('utf-8'))
        elif filtro == "hgray":
            print("Procesando horizontal a BYN")
            image_processing.process_images_mirror_horizontal_gray(input_dir.encode('utf-8'), os.path.join(input_dir, "HorizontalGray").encode('utf-8'))
        elif filtro == "hcolor":
            print("Procesando horizontal a Color")
            image_processing.process_images_mirror_horizontal_color(input_dir.encode('utf-8'), os.path.join(input_dir, "HorizontalColor").encode('utf-8'))
        elif filtro == "vgray":
            print("Procesando vertical a BYN")
            image_processing.process_images_mirror_vertical_gray(input_dir.encode('utf-8'), os.path.join(input_dir, "VerticalGray").encode('utf-8'))
        elif filtro == "vcolor":
            print("Procesando vertical a Color")
            image_processing.process_images_mirror_vertical_color(input_dir.encode('utf-8'), os.path.join(input_dir, "VerticalColor").encode('utf-8'))
        else:
            print("Filtro no reconocido")

#Ejecución principal
if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.exec()
