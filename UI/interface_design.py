import sys, locale
from PyQt6 import QtCore, QtWidgets

class Ui_Dialog(object):
    #Generar la IU
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(390, 300)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Policy.Fixed, QtWidgets.QSizePolicy.Policy.Fixed)
        Dialog.setSizePolicy(sizePolicy)
        Dialog.setMinimumSize(QtCore.QSize(390, 300))
        Dialog.setMaximumSize(QtCore.QSize(390, 300))
        
        #Botones de aceptación y cancelación
        self.buttonBox = QtWidgets.QDialogButtonBox(parent=Dialog)
        self.buttonBox.setGeometry(QtCore.QRect(20, 230, 350, 30))
        self.buttonBox.setOrientation(QtCore.Qt.Orientation.Horizontal)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.StandardButton.Cancel | QtWidgets.QDialogButtonBox.StandardButton.Ok)
        
        self.labelImage = QtWidgets.QLabel(parent=Dialog)
        self.labelImage.setGeometry(QtCore.QRect(20, 30, 350, 20))

        self.fileLineEdit = QtWidgets.QLineEdit(parent=Dialog)
        self.fileLineEdit.setGeometry(QtCore.QRect(20, 60, 300, 30))
        
        self.browseButton = QtWidgets.QPushButton("...", parent=Dialog)
        self.browseButton.setGeometry(QtCore.QRect(330, 60, 40, 30))
        self.browseButton.clicked.connect(self.open_file_dialog)

        self.labelFilter = QtWidgets.QLabel(parent=Dialog)
        self.labelFilter.setGeometry(QtCore.QRect(20, 130, 170, 30))

        self.comboBox = QtWidgets.QComboBox(parent=Dialog)
        self.comboBox.setGeometry(QtCore.QRect(210, 130, 160, 32))

        self.spinBox = QtWidgets.QSpinBox(parent=Dialog)
        self.spinBox.setGeometry(QtCore.QRect(300, 180, 70, 30))

        self.labelKernel = QtWidgets.QLabel(parent=Dialog)
        self.labelKernel.setGeometry(QtCore.QRect(20, 180, 170, 30))

        self.retranslateUi(Dialog)
        self.buttonBox.accepted.connect(Dialog.accept)
        self.buttonBox.rejected.connect(Dialog.reject)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    #Traducciones
    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        lang, _ = locale.getlocale()

        if lang == "es" or lang == "es_MX":
            Dialog.setWindowTitle(_translate("Dialog", "Procesamiento de imágenes"))
            self.labelImage.setText(_translate("Dialog", "Selecciona la imagen a procesar"))
            self.labelFilter.setText(_translate("Dialog", "Selecciona el tipo de filtro"))
            self.labelKernel.setText(_translate("Dialog", "Selecciona el valor del núcleo"))
            self.browseButton.setText(_translate("Dialog", "..."))

            self.comboBox.clear()
            self.comboBox.addItems([
                _translate("Dialog", "Original"),
                _translate("Dialog", "Desenfoque"),
                _translate("Dialog", "En escala de grises"),
                _translate("Dialog", "Horizontal a blanco y negro"),
                _translate("Dialog", "Horizontal a color"),
                _translate("Dialog", "Vertical a blanco y negro"),
                _translate("Dialog", "Vertical a color")
            ])
        else:
            Dialog.setWindowTitle(_translate("Dialog", "Image Processing"))
            self.labelImage.setText(_translate("Dialog", "Select the image to process"))
            self.labelFilter.setText(_translate("Dialog", "Select the filter type"))
            self.labelKernel.setText(_translate("Dialog", "Select the kernel value"))
            self.browseButton.setText(_translate("Dialog", "..."))

            self.comboBox.clear()
            self.comboBox.addItems([
                _translate("Dialog", "Original"),
                _translate("Dialog", "Blurred"),
                _translate("Dialog", "Gray Scale"),
                _translate("Dialog", "Horizontal Gray"),
                _translate("Dialog", "Horizontal Color"),
                _translate("Dialog", "Vertical Gray"),
                _translate("Dialog", "Vertical Color")
            ])

    #Abrir diálogo de selección de imágenes
    def open_file_dialog(self):
        file_name, _ = QtWidgets.QFileDialog.getOpenFileName(
            None,
            "Selecciona la imagen",
            "",
            "Archivos PNG (*.png);;Archivos JPG (*.jpg);;Archivos JPEG (*.jpeg);;Archivos BMP (*.bmp)"
        )
        if file_name:
            #Establece la ruta del archivo seleccionado en el QLineEdit
            self.fileLineEdit.setText(file_name)

#Ejecución principal
if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.exec()