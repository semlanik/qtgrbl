import QtQuick
import QtQuick.Controls

Popup {
    id: root
    anchors.centerIn: parent
    width: 200
    height: 300
    modal: true
    focus: true
    onClosed: {
        errorText.text = ""
        GrblSerial.clearError()
    }
    contentItem: Item {
        height: childrenRect.width
        width: childrenRect.height
        Text {
            id: errorText
            text: "No error"
        }
        Button {
            anchors.top: errorText.bottom
            text: "Clear and reset"
            onClicked: root.close()
        }
    }
    Connections {
        target: GrblSerial
        function onGrblErrorChanged() {
            if (GrblSerial.grblError !== 0) {
                errorText.text = "Error code:" + GrblSerial.errorCode
                root.open()
            } else {
                errorText.text = "No error"
            }
        }
    }
}
