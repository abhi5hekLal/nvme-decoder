import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1120
    height: 820
    minimumWidth: 680
    minimumHeight: 620
    visible: true
    title: "NVMe Decoder"
    color: "#111318"

    property string commandSet: "admin"
    property string decodedCommandSet: "admin"
    property bool hasResult: false
    property bool inputVisible: true
    property string commandName: ""
    property string commandUse: ""
    property string errorText: ""
    property var decodedFields: []

    function showInput() {
        inputVisible = true
        Qt.callLater(function() { commandInput.forceActiveFocus() })
    }

    function decodeInput() {
        const result = decoder.decodeCommand(commandInput.text, commandSet)
        if (!result.ok) {
            hasResult = false
            decodedFields = []
            errorText = result.error
            inputVisible = true
            return
        }

        errorText = ""
        commandName = result.commandName
        commandUse = result.use
        decodedFields = result.fields
        decodedCommandSet = commandSet
        hasResult = true
        inputVisible = false
        Qt.callLater(function() { reopenInputButton.forceActiveFocus() })
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 72
            color: "#151820"
            border.color: "#292d38"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                spacing: 9

                Rectangle {
                    Layout.preferredWidth: 36
                    Layout.preferredHeight: 36
                    radius: 11
                    gradient: Gradient {
                        GradientStop { position: 0; color: "#37d5ad" }
                        GradientStop { position: 1; color: "#168aaf" }
                    }

                    Label {
                        anchors.centerIn: parent
                        text: "N"
                        color: "#081411"
                        font.pixelSize: 19
                        font.weight: Font.Black
                    }
                }

                ColumnLayout {
                    spacing: 0

                    Label {
                        text: "NVMe Decoder"
                        color: "#f1f3f8"
                        font.pixelSize: 15
                        font.weight: Font.DemiBold
                    }

                    Label {
                        text: "COMMAND WORKSPACE"
                        color: "#8b93a3"
                        font.pixelSize: 9
                        font.letterSpacing: 1.3
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Label {
                    visible: window.width >= 820
                    text: "16 DWORDS"
                    color: "#8b93a3"
                    font.pixelSize: 10
                    font.letterSpacing: 1
                }

                Rectangle {
                    radius: 10
                    color: "#1d222c"
                    border.color: "#343a48"
                    implicitWidth: 140
                    implicitHeight: 38

                    Row {
                        id: setButtons
                        anchors.fill: parent
                        anchors.margins: 3
                        spacing: 0

                        Button {
                            id: adminButton
                            width: 67
                            height: parent.height
                            text: "Admin"
                            onClicked: window.commandSet = "admin"
                            background: Rectangle {
                                radius: 7
                                color: window.commandSet === "admin" ? "#283f3d" : "transparent"
                            }
                            contentItem: Text {
                                text: adminButton.text
                                color: window.commandSet === "admin" ? "#c7ffeb" : "#a0a7b5"
                                font.pixelSize: 13
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Button {
                            id: nvmButton
                            width: 67
                            height: parent.height
                            text: "NVM"
                            onClicked: window.commandSet = "nvm"
                            background: Rectangle {
                                radius: 7
                                color: window.commandSet === "nvm" ? "#283f3d" : "transparent"
                            }
                            contentItem: Text {
                                text: nvmButton.text
                                color: window.commandSet === "nvm" ? "#c7ffeb" : "#a0a7b5"
                                font.pixelSize: 13
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }
            }
        }

        Flickable {
            id: resultScroller
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 0
            Layout.preferredHeight: 0
            contentWidth: width
            contentHeight: resultColumn.implicitHeight
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar { }

            ColumnLayout {
                id: resultColumn
                width: Math.min(920, Math.max(0, resultScroller.width - 48))
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 38
                spacing: 24

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 6

                            Label {
                                text: "DECODE OUTPUT"
                                color: "#53d9b4"
                                font.pixelSize: 10
                                font.weight: Font.Bold
                                font.letterSpacing: 1.5
                            }

                            Label {
                                text: hasResult ? "Command details" : "Decode an NVMe command"
                                color: "#f1f3f8"
                                font.pixelSize: 27
                                font.weight: Font.DemiBold
                            }

                            Label {
                                Layout.fillWidth: true
                                Layout.minimumWidth: 0
                                text: hasResult
                                      ? "Each field includes its decoded value, bit range, and meaning."
                                      : "Paste the 16 command dwords below. Choose Admin or NVM to match the command set."
                                color: "#9ca5b5"
                                wrapMode: Text.WordWrap
                                font.pixelSize: 13
                            }
                        }

                        Rectangle {
                            visible: hasResult
                            Layout.fillWidth: true
                            Layout.minimumWidth: 0
                            implicitHeight: resultBody.implicitHeight
                            radius: 17
                            color: "#1a1e27"
                            border.color: "#303744"

                            ColumnLayout {
                                id: resultBody
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                spacing: 0

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 9
                                    Layout.leftMargin: 24
                                    Layout.rightMargin: 24
                                    Layout.topMargin: 21
                                    Layout.bottomMargin: 19

                                    RowLayout {
                                        Layout.fillWidth: true

                                        Rectangle {
                                            radius: 6
                                            color: "#19342f"
                                            border.color: "#28564a"
                                            implicitWidth: commandSetBadge.implicitWidth + 18
                                            implicitHeight: commandSetBadge.implicitHeight + 10

                                            Label {
                                                id: commandSetBadge
                                                anchors.centerIn: parent
                                                text: decodedCommandSet === "admin" ? "ADMIN COMMAND" : "NVM COMMAND"
                                                color: "#72e0bd"
                                                font.pixelSize: 9
                                                font.weight: Font.Bold
                                                font.letterSpacing: 1
                                            }
                                        }

                                        Item { Layout.fillWidth: true }

                                        Label {
                                            text: "●  DECODED"
                                            color: "#72e0bd"
                                            font.pixelSize: 9
                                            font.weight: Font.DemiBold
                                            font.letterSpacing: 0.8
                                        }
                                    }

                                    Label {
                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 0
                                        text: commandName
                                        color: "#f1f3f8"
                                        wrapMode: Text.WordWrap
                                        font.pixelSize: 21
                                        font.weight: Font.DemiBold
                                    }

                                    Label {
                                        Layout.fillWidth: true
                                        Layout.minimumWidth: 0
                                        text: commandUse
                                        color: "#a8b0be"
                                        wrapMode: Text.WordWrap
                                        font.pixelSize: 13
                                    }
                                }

                                Rectangle {
                                    Layout.fillWidth: true
                                    height: 1
                                    color: "#303744"
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 0
                                    Layout.leftMargin: 24
                                    Layout.rightMargin: 24
                                    Layout.topMargin: 14
                                    Layout.bottomMargin: 15

                                    RowLayout {
                                        Layout.fillWidth: true
                                        Layout.bottomMargin: 8

                                        Label {
                                            text: "Decoded fields"
                                            color: "#e8ebf2"
                                            font.pixelSize: 13
                                            font.weight: Font.DemiBold
                                        }

                                        Item { Layout.fillWidth: true }

                                        Label {
                                            text: decodedFields.length + " FIELDS"
                                            color: "#818b9c"
                                            font.pixelSize: 9
                                            font.letterSpacing: 1
                                        }
                                    }

                                    Repeater {
                                        model: decodedFields

                                        delegate: ColumnLayout {
                                            required property var modelData
                                            Layout.fillWidth: true
                                            spacing: 0

                                            Rectangle {
                                                Layout.fillWidth: true
                                                height: 1
                                                color: "#303744"
                                            }

                                            ColumnLayout {
                                                Layout.fillWidth: true
                                                spacing: 7
                                                Layout.topMargin: 11
                                                Layout.bottomMargin: 11

                                                RowLayout {
                                                    Layout.fillWidth: true
                                                    spacing: 14

                                                    Label {
                                                        Layout.fillWidth: true
                                                        Layout.minimumWidth: 0
                                                        text: modelData.name
                                                        color: "#aeb7c6"
                                                        font.pixelSize: 12
                                                        wrapMode: Text.WordWrap
                                                    }

                                                    Label {
                                                        Layout.maximumWidth: 320
                                                        Layout.minimumWidth: 0
                                                        text: modelData.value
                                                        color: "#f1f3f8"
                                                        font.pixelSize: 13
                                                        font.weight: Font.DemiBold
                                                        horizontalAlignment: Text.AlignRight
                                                        wrapMode: Text.WordWrap
                                                    }
                                                }

                                                RowLayout {
                                                    Layout.fillWidth: true
                                                    spacing: 9

                                                    Rectangle {
                                                        Layout.alignment: Qt.AlignTop
                                                        radius: 5
                                                        color: "#222833"
                                                        border.color: "#343c4a"
                                                        implicitWidth: fieldLocation.implicitWidth + 12
                                                        implicitHeight: fieldLocation.implicitHeight + 6

                                                        Label {
                                                            id: fieldLocation
                                                            anchors.centerIn: parent
                                                            text: modelData.location
                                                            color: "#9da8ba"
                                                            font.family: "monospace"
                                                            font.pixelSize: 10
                                                        }
                                                    }

                                                    Label {
                                                        Layout.fillWidth: true
                                                        Layout.minimumWidth: 0
                                                        text: modelData.meaning
                                                        color: "#858f9f"
                                                        font.pixelSize: 11
                                                        wrapMode: Text.WordWrap
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        Label {
                            visible: errorText.length > 0
                            Layout.fillWidth: true
                            text: errorText
                            color: "#ffb4ab"
                            wrapMode: Text.WordWrap
                            padding: 14
                            background: Rectangle {
                                radius: 9
                                color: "#302126"
                                border.color: "#6b414b"
                            }
                        }
            }
        }

        Rectangle {
            visible: inputVisible
            Layout.fillWidth: true
            Layout.preferredHeight: 164
            Layout.minimumHeight: 164
            Layout.maximumHeight: 164
                    color: "#111318"

                    ColumnLayout {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 16
                        width: Math.min(850, parent.width - 48)
                        spacing: 7

                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: composerColumn.implicitHeight + 24
                            radius: 15
                            color: "#191d25"
                            border.color: commandInput.activeFocus ? "#42cba6" : "#343a48"

                            ColumnLayout {
                                id: composerColumn
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                ScrollView {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 64
                                    clip: true

                                    TextArea {
                                        id: commandInput
                                        width: parent.width
                                        placeholderText: "Paste 16 hexadecimal dwords (CDW0 through CDW15), separated by spaces or new lines…"
                                    placeholderTextColor: "#838c9b"
                                    color: "#edf0f6"
                                        selectedTextColor: "#ffffff"
                                    selectionColor: "#265c4e"
                                        wrapMode: TextEdit.Wrap
                                        selectByMouse: true
                                        font.family: "monospace"
                                        font.pixelSize: 12
                                        background: Item { }
                                        onTextChanged: errorText = ""
                                        Keys.onReturnPressed: function(event) {
                                            window.decodeInput()
                                            event.accepted = true
                                        }
                                    }
                                }

                                Rectangle {
                                    Layout.fillWidth: true
                                    height: 1
                                    color: "#343a48"
                                }

                                RowLayout {
                                    Layout.fillWidth: true

                                    Label {
                                        Layout.fillWidth: true
                                        text: "16 dwords · 32-bit hexadecimal values · paste from your trace"
                                        color: "#99a2b1"
                                        font.pixelSize: 11
                                        elide: Text.ElideRight
                                    }

                                    Button {
                                        id: decodeButton
                                        text: "Decode command  ↑"
                                        onClicked: window.decodeInput()
                                        background: Rectangle {
                                            radius: 9
                                            gradient: Gradient {
                                                GradientStop { position: 0; color: decodeButton.down ? "#159d81" : "#28bd98" }
                                                GradientStop { position: 1; color: decodeButton.down ? "#11728d" : "#1989a5" }
                                            }
                                        }
                                        contentItem: Text {
                                            text: decodeButton.text
                                            color: "#071712"
                                            font.pixelSize: 13
                                            font.weight: Font.DemiBold
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            leftPadding: 10
                                            rightPadding: 10
                                        }
                                    }
                                }
                            }
                        }

                        Label {
                            Layout.fillWidth: true
                            text: "Input is parsed and decoded by the C++ backend."
                            color: "#747f8f"
                            font.pixelSize: 10
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
        }

        Rectangle {
            visible: !inputVisible
            Layout.fillWidth: true
            Layout.preferredHeight: 58
            Layout.minimumHeight: 58
            Layout.maximumHeight: 58
            color: "#111318"

            Button {
                id: reopenInputButton
                anchors.centerIn: parent
                focus: true
                text: "+  Decode another command"
                onClicked: window.showInput()
                Keys.onPressed: function(event) {
                    if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                        window.showInput()
                        event.accepted = true
                    }
                }
                background: Rectangle {
                    radius: 9
                    color: reopenInputButton.down ? "#202833" : "#191d25"
                    border.color: "#3b4554"
                }
                contentItem: Text {
                    text: reopenInputButton.text
                    color: "#cbd4e3"
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 13
                    rightPadding: 13
                }
            }
        }
    }
}
