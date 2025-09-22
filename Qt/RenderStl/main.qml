import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.0
import QtQuick.Controls 1.1
import OpenGLUnderQML 1.0
import QtQuick.Controls.Styles 1.4

Item {
    id: item
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight

    /***********************
     * Signals to Main application
     **********************/
    signal fileOpenedSignal(string fileName);
    signal generateGCodeSignal(string outFileName, int printer_id);
    signal generateSupportSignal();
    signal setSupportAndQuality(int supportSelected, int qualitySelected);

    property int supportSelected: 0;
    property int qualitySelected: 0;
    property int printer_id: 0;
    property string guiMessage: msg.gui_message;

    /***********************
     * Functions definitions
     **********************/
    function handleMouseEvents(mouseX, mouseY, event) {
        /* Hack! */
        selectSupportGrid.visible = false
        selectQualityGrid.visible = false
        switch(event) {
        case 0:
            glItem.xPos = mouseX;
            glItem.yPos = mouseY;
            break;
        case 1:
            if (mouseY > topMenuGrid.height) {
                glItem.resetPos = true;
            }
            break;
        case 2:
            glItem.yZoom = mouseY;
            break;
        default:
        }
    }

    function onClickedTopMenu(index) {
        switch(index) {
        case 0:
            /* Open File */
            openDialog.open()
            openDialog.visible = true
            break;
        case 1:
            /* Size */
            scaleMenuWrapper.visible = !scaleMenuWrapper.visible
            scaleMenuCloseWrapper.visible = !scaleMenuCloseWrapper.visible
            break;
        case 2:
            /* Rotate */
            rotateMenuGrid.visible = !rotateMenuGrid.visible
            rotateMenuCloseWrapper.visible = !rotateMenuCloseWrapper.visible
            rotateMenuTitleWrapper.visible = !rotateMenuTitleWrapper.visible
            break;
        case 3:
            /* Support */
            item.generateSupportSignal();
            selectSupportGrid.visible = !selectSupportGrid.visible
            break;
        case 4:
            /* Quality */
            selectQualityGrid.visible = !selectQualityGrid.visible
            break;
        case 5:
            /* Save */
            saveDialog.open();
            break;
        case 6:
            /* Print */
            break;
        default:
            console.log("Something else")
        }
    }

    function functionOnClickedRotate(index, up_or_down) {
        /* 0 - up, 1 - down
         * glItem values X up 0, X down 1, Y up 2, Y down 3,
         * Z up 4, Z down 5 */
        switch(index) {
        case 0:
            if (up_or_down === 0)
                glItem.rotateObj = 0;
            else
                glItem.rotateObj = 1;
            break;
        case 1:
            if (up_or_down === 0)
                glItem.rotateObj = 2;
            else
                glItem.rotateObj = 3;
            break;
        case 2:
            if (up_or_down === 0)
                glItem.rotateObj = 4;
            else
                glItem.rotateObj = 5;
            break;
        default:
            break;
        }
    }

    function functionOnClickedScale(up_or_down) {
        /* 0 - up and 1 is down.
         * glItem values continued from rotation.
         * 6 is scale up and 7 is scale down */
        switch(up_or_down) {
        case 0:
            glItem.rotateObj = 6;
            break;
        case 1:
            glItem.rotateObj = 7;
            break;
        default:
            break;
        }
    }

    /***********************
     * Dummy rectangle to capture mouse events
     **********************/
    Rectangle {
        id: wrapperMouse
        opacity: 0
        anchors.fill: parent
        MouseArea {
            anchors.fill: parent
            onPositionChanged: {
                handleMouseEvents(mouseX, mouseY, 0);
            }
            onReleased: {
                handleMouseEvents(mouseX, mouseY, 1);
            }
            onWheel: {
                handleMouseEvents(0, wheel.angleDelta.y, 2);
                glItem.yZoom = wheel.angleDelta.y
            }
            onClicked: {
                handleMouseEvents(mouseX, mouseY, 1);
            }
        }
        PinchArea {
            anchors.fill: parent
            onSmartZoom: {
                console.log("Smart zoom detected")
            }
        }
    }

    /***********************
     * Select the printer. For testing.
     **********************/
    ComboBox {
        id: selectPrinter
        anchors.left: parent.left
        anchors.top : parent.top
        anchors.margins: 2
        currentIndex: 0
        width: 100
        visible: true
        model: [ "Printer1", "Printer2" ]
        onCurrentIndexChanged: {
            printer_id = currentIndex;
        }
    }

    /* Flickable {
        anchors.fill: parent

        contentWidth: parent.width * 2
        contentHeight: parent.height * 2

        ScrollBar.horizontal: ScrollBar { id: hbar; active: vbar.active }
        ScrollBar.vertical: ScrollBar { id: vbar; active: hbar.active }
    } */

    /***********************
     * OpenGL canvas
     **********************/
    OpenGLQuickItem {
        id: glItem
        anchors.fill: parent
    }


    /***********************
     * Top Menu
     **********************/
    GridView {
        id: topMenuGrid
        anchors.horizontalCenter: parent.horizontalCenter
        width: 420; height:55
        cellWidth: 60; cellHeight: this.height

        Component {
            id: topMenuComponent
            Rectangle {
                id: topMenuWrapper
                height: topMenuGrid.cellHeight;
                width: topMenuGrid.cellWidth-2;
                radius: 3; smooth: true
                color: Qt.rgba(0.5, 0.5, 0.5, 1)
                focus: true
                MouseArea {
                    anchors.fill: topMenuWrapper
                    preventStealing: true
                    hoverEnabled: true
                    onClicked: {
                        onClickedTopMenu(index)
                    }
                    onEntered: {
                        iconImage.source = iconsourceOnFocus
                        topMenuWrapper.height = 75
                    }
                    onExited: {
                        iconImage.source = iconsource
                        topMenuWrapper.height = topMenuGrid.cellHeight
                    }

                }
                Image {
                    id: iconImage
                    source: iconsource;
                    anchors.centerIn: topMenuWrapper
                }
                Text {
                    id: iconText
                    text: name;
                    color: "#FFFFFF"
                    font.pointSize: 10
                    anchors.top: iconImage.bottom;
                    anchors.horizontalCenter: iconImage.horizontalCenter
                }
            }
        }

        model: MenuItems {}
        delegate: topMenuComponent
    }

    /***********************
     * File Dialogs. Open and Save
     **********************/
    FileDialog {
        id: openDialog
        title: "Please choose a STL file"
        //folder: shortcuts.home
        nameFilters: [ "STL files (*.stl)", "All files (*)" ]
        onAccepted: {
            item.fileOpenedSignal (openDialog.fileUrls)
        }
        onRejected: {
        }
    }

    FileDialog {
        id: saveDialog
        title: "Please choose a location to save the G Code"
        //folder: shortcuts.home
        selectExisting: false
        onAccepted: {
            item.generateGCodeSignal(saveDialog.fileUrls, selectPrinter.index)
        }
        onRejected: {
        }
    }

    /***********************
     * Rotate Menu
     **********************/
    GridView {
        id: rotateMenuGrid
        anchors.top: rotateMenuTitleWrapper.bottom
        anchors.left: item.left; anchors.leftMargin: 10

        height: item.height/3; width: item.width/20
        cellWidth: rotateMenuGrid.width; cellHeight: rotateMenuGrid.height/3
        visible: false

        Component {
            id: rotateMenuComponent
            Rectangle {
                id: rotateMenuWrapper
                height: rotateMenuGrid.cellHeight; width: rotateMenuGrid.cellWidth
                radius: 3; smooth: true
                color: Qt.rgba(0.9, 0.5, 0.5, 1.0);
                focus: true;
                MouseArea {
                    anchors.fill: rotateMenuWrapper
                    hoverEnabled: true
                }
                Text {
                    id: rotateText

                    anchors.left: rotateMenuWrapper.left
                    anchors.verticalCenter: rotateMenuWrapper.verticalCenter
                    anchors.right: rotateMenuWrapper.horizontalCenter
                    anchors.margins: 5

                    text: name;
                    color: "#FFFFFF"
                    font.pointSize: 15
                }
                Image {
                    id: rotateImageUp
                    source: imgSrcRotUp
                    anchors.horizontalCenter: rotateMenuWrapper.horizontalCenter
                    anchors.top: rotateMenuWrapper.top
                    anchors.bottom: rotateMenuWrapper.verticalCenter
                    anchors.margins: 1
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: rotateImageUp.source = imgSrcRotUpOnFocus
                        onExited: rotateImageUp.source = imgSrcRotUp
                        onClicked: functionOnClickedRotate(index, 0)
                    }
                }
                Image {
                    id: rotateImageDown
                    source: imgSrcRotDown
                    anchors.horizontalCenter: rotateMenuWrapper.horizontalCenter
                    anchors.top: rotateMenuWrapper.verticalCenter
                    anchors.margins: 1
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: rotateImageDown.source = imgSrcRotDownOnFocus
                        onExited: rotateImageDown.source = imgSrcRotDown
                        onClicked: functionOnClickedRotate(index, 1)
                    }
                }
            }
        }
        model: RotateMenu {}
        delegate: rotateMenuComponent
    } //Grid

    Rectangle {
        id: rotateMenuCloseWrapper
        height: rotateMenuGrid.cellHeight/4; width: rotateMenuGrid.width

        anchors.top: rotateMenuGrid.bottom
        anchors.left: rotateMenuGrid.left
        radius: 3; smooth: true

        color: Qt.rgba(1.0, 0.4, 0.4, 1.0);
        visible: false
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:{
                rotateMenuCloseWrapper.color = Qt.rgba(1.0, 0.3, 0.3, 1.0)
                rotateMenuCloseLabel.font.pointSize = 20
            }
            onExited: {
                rotateMenuCloseWrapper.color = Qt.rgba(1.0, 0.4, 0.4, 1.0)
                rotateMenuCloseLabel.font.pointSize = 15
            }
            onClicked: {
                rotateMenuCloseWrapper.visible = false
                rotateMenuGrid.visible = false
                rotateMenuTitleWrapper.visible = false
            }
        }
        Text {
            id: rotateMenuCloseLabel
            text: "Close"
            anchors.centerIn: rotateMenuCloseWrapper
            color: "#FFFFFF"; font.pointSize: 15
        }
    }
    Rectangle {
        id: rotateMenuTitleWrapper
        height: rotateMenuGrid.cellHeight/4; width: rotateMenuGrid.width

        anchors.top: selectQualityGrid.bottom
        anchors.left: rotateMenuGrid.left
        anchors.topMargin: 5
        radius: 3; smooth: true

        color: Qt.rgba(1.0, 0.4, 0.4, 1.0);
        visible: false
        Text {
            id: rotateMenuTitleLabel
            text: "Rotate"
            anchors.centerIn: rotateMenuTitleWrapper
            color: "#FFFFFF"; font.pointSize: 15
        }
    }

    /***********************
     * Scale Menu
     **********************/
    Rectangle {
        id: scaleMenuWrapper
        height: item.height/9; width: item.width/20

        anchors.left: item.left
        anchors.top: rotateMenuCloseWrapper.bottom
        anchors.leftMargin: 10; anchors.topMargin: 10
        radius: 3; smooth: true

        color: Qt.rgba(0.9, 0.5, 0.5, 1.0);
        visible: false
        Text {
            id: scaleMenuTitleLabel
            text: "Scale"
            anchors.centerIn: scaleMenuWrapper
            color: "#FFFFFF"; font.pointSize: 15
        }
        Image {
            id: scaleImageUp
            source: "assets/icons/arrow_up_white_2x.png"
            anchors.horizontalCenter: scaleMenuWrapper.horizontalCenter
            anchors.top: scaleMenuWrapper.top
            anchors.bottom: scaleMenuWrapper.verticalCenter
            anchors.margins: 1
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: scaleImageUp.source = "assets/icons/arrow_up_white_3x.png"
                onExited: scaleImageUp.source = "assets/icons/arrow_up_white_2x.png"
                onClicked: functionOnClickedScale(0)
            }
        }
        Image {
            id: scaleImageDown
            source: "assets/icons/arrow_down_white_2x.png"
            anchors.horizontalCenter: scaleMenuWrapper.horizontalCenter
            anchors.top: scaleMenuWrapper.verticalCenter
            anchors.bottom: scaleMenuWrapper.bottom
            anchors.margins: 1
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: scaleImageDown.source = "assets/icons/arrow_down_white_3x.png"
                onExited: scaleImageDown.source = "assets/icons/arrow_down_white_2x.png"
                onClicked: functionOnClickedScale(1)
            }
        }
    }

    Rectangle {
        id: scaleMenuCloseWrapper
        height: rotateMenuGrid.cellHeight/4; width: rotateMenuGrid.width

        anchors.top: scaleMenuWrapper.bottom
        anchors.right: scaleMenuWrapper.right
        radius: 3; smooth: true

        color: Qt.rgba(0.9, 0.4, 0.4, 1.0);
        visible: false
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered:{
                scaleMenuCloseWrapper.color = Qt.rgba(1.0, 0.3, 0.3, 1.0)
                scaleMenuCloseLabel.font.pointSize = 20
            }
            onExited: {
                scaleMenuCloseWrapper.color = Qt.rgba(0.9, 0.4, 0.4, 1.0)
                scaleMenuCloseLabel.font.pointSize = 15
            }
            onClicked: {
                scaleMenuCloseWrapper.visible = false
                scaleMenuWrapper.visible = false
            }
        }
        Text {
            id: scaleMenuCloseLabel
            text: "Close"
            anchors.centerIn: scaleMenuCloseWrapper
            color: "#FFFFFF"; font.pointSize: 15
        }
    }

    /**************************
     * Support - Select option
     *************************/
    GroupBox {
        id: selectSupportGrid
        anchors.left: item.left; anchors.top: selectPrinter.bottom
        visible: false;

        Column {
            ExclusiveGroup { id: selectSupport }
            Rectangle {
                id: selectSupportTitle
                height: topMenuGrid.cellHeight * 0.75; width: topMenuGrid.cellWidth * 1.5;
                color: Qt.rgba(0.9, 0.4, 0.4, 1.0); radius: 10;
                Text {
                    anchors.centerIn: parent
                    text: "Select\nSupport"
                    color: "#FFFFFF"; font.pointSize: 15
                }
            }

            Rectangle {
                id: radioButtonDefaultWrapper
                height: topMenuGrid.cellHeight * 0.75; width: topMenuGrid.cellWidth * 1.5;
                color: Qt.rgba(0.5, 0.5, 0.5, 1); radius: 10;
                RadioButton {
                    id: radioButtonDefault
                    anchors.centerIn: radioButtonDefaultWrapper
                    checked: true
                    exclusiveGroup: selectSupport
                    onClicked: {
                        if (supportSelected != 0) {
                            supportSelected = 0;
                            setSupportAndQuality(supportSelected,
                                             qualitySelected);
                        }
                    }
                    style: RadioButtonStyle {
                        label: Text {
                            text: "Default"
                            color: "#FFFFFF"; font.pointSize: 15
                        }
                    }
                }
            }
            Rectangle {
                id: radioButtonAllWrapper
                height: topMenuGrid.cellHeight  * 0.75; width: topMenuGrid.cellWidth * 1.5;
                color: Qt.rgba(0.5, 0.5, 0.5, 1); radius: 10;
                RadioButton {
                    id: radioButtonAll
                    anchors.centerIn: radioButtonAllWrapper
                    exclusiveGroup: selectSupport
                    onClicked: {
                        if (supportSelected != 1) {
                            supportSelected = 1;
                            setSupportAndQuality(supportSelected,
                                             qualitySelected);
                        }
                    }
                    style: RadioButtonStyle {
                        label: Text {
                            text: "All"
                            color: "#FFFFFF"; font.pointSize: 13
                        }
                    }
                }
            }
            MouseArea {
                hoverEnabled: true
                onEntered: selectSupportGrid.visible = true
                onExited: selectSupportGrid.visible = false
            }
        }
    }

    /**************************
     * Quality - Select option
     *************************/
    GroupBox {
        id: selectQualityGrid
        anchors.left: item.left
        anchors.top: selectSupportGrid.bottom
        visible: false; //anchors.margins: 5;

        Column {
            ExclusiveGroup { id: selectQuality }
            Rectangle {
                id: selectQualityTitle
                height: topMenuGrid.cellHeight * 0.75; width: topMenuGrid.cellWidth * 1.5;
                color: Qt.rgba(0.9, 0.4, 0.4, 1.0); radius: 10;
                Text {
                    anchors.centerIn: parent
                    text: "Select Print\nQuality"
                    color: "#FFFFFF"; font.pointSize: 12
                }
            }

            Rectangle {
                id: qualityNormalWrapper
                height: topMenuGrid.cellHeight * 0.75; width: topMenuGrid.cellWidth * 1.5;
                color: Qt.rgba(0.5, 0.5, 0.5, 1); radius: 10;
                RadioButton {
                    id: qualityNormal
                    anchors.centerIn: qualityNormalWrapper
                    checked: true
                    exclusiveGroup: selectQuality
                    onClicked: {
                        if (qualitySelected != 0) {
                            qualitySelected = 0;
                            setSupportAndQuality(supportSelected,
                                                 qualitySelected);
                        }
                    }

                    style: RadioButtonStyle {
                        label: Text {
                            text: "Normal"
                            color: "#FFFFFF"; font.pointSize: 15
                        }
                    }
                }
            }
            Rectangle {
                id: qualityHighWrapper
                height: topMenuGrid.cellHeight  * 0.75; width: topMenuGrid.cellWidth * 1.5;
                color: Qt.rgba(0.5, 0.5, 0.5, 1); radius: 10;
                RadioButton {
                    id: qualityHigh
                    anchors.centerIn: qualityHighWrapper
                    exclusiveGroup: selectQuality
                    onClicked: {
                        if (qualitySelected != 1) {
                            qualitySelected = 1;
                            setSupportAndQuality(supportSelected,
                                                 qualitySelected);
                        }
                    }
                    style: RadioButtonStyle {
                        label: Text {
                            text: "High"
                            color: "#FFFFFF"; font.pointSize: 15
                        }
                    }
                }
            } /* Rectangle */
            MouseArea {
                hoverEnabled: true
                onEntered: selectQualityGrid.visible = true
                onExited: selectQualityGrid.visible = false
            }
        } /* Column */
    } /* GroupBox */

    /**************************
    * Messaging Box
    **************************/
    TextField {
        id: guiMessageId;
        anchors.bottom: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter;
        width: parent.width/2; height: parent.height/32;

        readOnly: true;
        horizontalAlignment: TextInput.AlignHCenter;
        style: TextFieldStyle {
            background: Rectangle {
                height: guiMessageId.height
                width: guiMessageId.width
                color: Qt.rgba(0.45, 0.45, 0.45, 1)
                radius: 10
            }
            textColor: Qt.rgba(0.9, 0.6, 0.6, 1.0);
        }
        text: guiMessage
    }
} /* Closing Item */
