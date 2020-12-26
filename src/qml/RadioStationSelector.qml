import QtQuick 2.11
import PiRadio.RadioStation  1.0
import PiRadio.RadioStations 1.0

Item {
    id: radioSelector

    property RadioStation current: radioStations.current

    RadioStations {
        id: radioStations
    }

    ListView {
        id: viewRadioStations
        width: parent.width;
        height: parent.height
        spacing: 4
        model: radioStations.stations
        delegate:
            RadioStationButton {
                width: viewRadioStations.width
                height: 60
                radioStation: radioStations.stations[index]
                highlight: radioStations.stations[index] === radioStations.current
                onSelected: {
                    radioStations.current = radioStation;
                }
            }
    }
}

