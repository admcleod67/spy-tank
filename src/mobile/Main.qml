import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import com.spytank 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 480
    title: qsTr("iSpy Tank Remote")

    readonly property bool isConnected: tank.state === SocketState.ConnectedState

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        // Connection Header
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 10
            TextField {
                id: ipInput
                text: "10.10.1.1"
                placeholderText: "Tank IP"
                enabled: !isConnected
            }
            TextField {
                id: portInput
                text: "8150"
                placeholderText: "Port"
                enabled: !isConnected
            }
            Button {
                text: isConnected ? "Disconnect" : "Connect"
                onClicked: {
                    if (isConnected) {
                        tank.disconnectFromTank()
                        video.stop()
                    } else {
                        tank.connectToTank(ipInput.text, parseInt(portInput.text))
                        video.start(ipInput.text)
                    }
                }
            }
            Label {
                text: "Status: " + (isConnected ? "Connected" : "Disconnected")
                color: isConnected ? "green" : "red"
            }
        }

        // Main content area
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Video Feed
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "black"
                VideoOutput {
                    anchors.fill: parent
                    fillMode: VideoOutput.PreserveAspectFit
                    videoSink: videoProducer.videoSink
                }
            }

            // Controls
            ColumnLayout {
                Layout.minimumWidth: 200
                Layout.margins: 10
                spacing: 20

                // Driving Grid
                GridLayout {
                    columns: 3
                    Button { text: "Q"; Layout.fillWidth: true; onPressed: tank.sendCommand(SpyTank.Camera, SpyTank.Forward); onReleased: tank.sendCommand(SpyTank.Camera, SpyTank.Stop) }
                    Button { text: "W"; Layout.fillWidth: true; 
                        onPressed: { tank.sendCommand(SpyTank.Left, SpyTank.Forward); tank.sendCommand(SpyTank.Right, SpyTank.Forward) } 
                        onReleased: { tank.sendCommand(SpyTank.Left, SpyTank.Stop); tank.sendCommand(SpyTank.Right, SpyTank.Stop) }
                    }
                    Button { text: "E"; Layout.fillWidth: true; onPressed: tank.sendCommand(SpyTank.Camera, SpyTank.Back); onReleased: tank.sendCommand(SpyTank.Camera, SpyTank.Stop) }

                    Button { text: "A"; Layout.fillWidth: true; 
                        onPressed: { tank.sendCommand(SpyTank.Left, SpyTank.Back); tank.sendCommand(SpyTank.Right, SpyTank.Forward) }
                        onReleased: { tank.sendCommand(SpyTank.Left, SpyTank.Stop); tank.sendCommand(SpyTank.Right, SpyTank.Stop) }
                    }
                    Button { text: "S"; Layout.fillWidth: true; 
                        onPressed: { tank.sendCommand(SpyTank.Left, SpyTank.Back); tank.sendCommand(SpyTank.Right, SpyTank.Back) }
                        onReleased: { tank.sendCommand(SpyTank.Left, SpyTank.Stop); tank.sendCommand(SpyTank.Right, SpyTank.Stop) }
                    }
                    Button { text: "D"; Layout.fillWidth: true; 
                        onPressed: { tank.sendCommand(SpyTank.Left, SpyTank.Forward); tank.sendCommand(SpyTank.Right, SpyTank.Back) }
                        onReleased: { tank.sendCommand(SpyTank.Left, SpyTank.Stop); tank.sendCommand(SpyTank.Right, SpyTank.Stop) }
                    }
                }
                
                Item { Layout.fillHeight: true } // Spacer
            }
        }
    }
}
