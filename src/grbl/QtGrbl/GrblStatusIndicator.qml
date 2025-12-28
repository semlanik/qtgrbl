import QtQuick

Item {
    id: root
    width: indicator.width + 6
    height: indicator.height + 6
    Rectangle {
        id: indicator
        anchors.centerIn: parent
        height: 15
        width: 15
        radius: width/5
        color: {
            switch (GrblSerial.status) {
                case GrblSerial.Error:
                    return "red"
                case GrblSerial.Busy:
                    return "yellow"
                default:
                    return "green"
            }
        }
    }
}
