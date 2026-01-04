import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import QtGrbl

Popup {
    id: root
    property int defaultMargins: 5 // TODO: move to the generic place
    contentItem: Item {
        Row {
            id: toolbar
            height: childrenRect.height + root.defaultMargins * 2
            Button {
                anchors.margins: root.defaultMargins
                text: "Save as..."
                onClicked: saveAsDialog.open()

                FileDialog {
                    id: saveAsDialog
                    acceptLabel: qsTr("Save")
                    defaultSuffix: ".sgrbl"
                    fileMode: FileDialog.SaveFile
                    nameFilters: [ "GRBL setting files (*.sgrbl)" ]
                    onAccepted: function() {
                        GrblEngine.saveSettings(saveAsDialog.selectedFile)
                        saveAsDialog.selectedFile = "";
                    }
                }
            }
        }
        ListView {
            id: settingList
            anchors {
                top: toolbar.bottom
                bottom: bottomBar.top
                left: parent.left
                right: parent.right
            }
            clip: true

            ScrollBar.vertical: ScrollBar { }

            model: GrblEngine.settingsProxy
            DelegateChooser {
                id: chooser
                role: "type"
                DelegateChoice {
                    roleValue: GrblSettingsModel.Bool
                    SettingBoolView {
                        width: settingList.width
                        name: model.data.name
                        value: model.data.value
                        enabled: model.isValid
                        isChanged: model.isChanged
                        onClicked: function(checked) {
                            model.userValue = checked
                        }
                        onReset: {
                            model.userValue = null
                        }
                    }
                }
                DelegateChoice {
                    roleValue: GrblSettingsModel.Int
                    SettingIntView {
                        width: settingList.width
                        name: model.data.name
                        value: model.data.value
                        unit: model.data.unit
                        enabled: model.isValid
                        isChanged: model.isChanged
                        onValueEdited: function (text) {
                            model.userValue = value
                        }
                        onReset: {
                            model.userValue = null
                        }
                    }
                }
                DelegateChoice {
                    roleValue: GrblSettingsModel.Float
                    SettingFloatView {
                        width: settingList.width
                        name: model.data.name
                        value: model.data.value
                        unit: model.data.unit
                        enabled: model.isValid
                        isChanged: model.isChanged
                        onValueEdited: function (text) {
                            model.userValue = value
                        }
                        onReset: {
                            model.userValue = null
                        }
                    }
                }
                DelegateChoice {
                    roleValue: GrblSettingsModel.Mask
                    SettingMaskView {
                        width: settingList.width
                        name: model.data.name
                        value: model.data.value
                        enabled: model.isValid
                        isChanged: model.isChanged
                        onValueEdited: function (text) {
                            model.userValue = value
                        }
                        onReset: {
                            model.userValue = null
                        }
                    }
                }
            }

            delegate: chooser
        }

        Item {
            id: bottomBar
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            height: childrenRect.height + root.defaultMargins * 2
            Button {
                anchors {
                    right: parent.right
                    topMargin: root.defaultMargins
                }
                text: qsTr("Apply")
                enabled: GrblEngine.settings.hasChanges
                onClicked: {
                    GrblEngine.applySettings()
                }
            }

            Button {
                anchors {
                    topMargin: defaultMargins
                    left: parent.left
                }
                text: qsTr("Cancel")
                onClicked: root.close()
            }
        }
    }

    onClosed: {
        GrblEngine.settings.resetChanges()
    }
}
