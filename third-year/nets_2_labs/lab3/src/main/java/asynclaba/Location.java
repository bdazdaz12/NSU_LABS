package asynclaba;


import java.util.ArrayList;
import java.util.List;

public final class Location {

    String latitude;
    String longitude;
    String locationName;
    String country;
    String city;
    String street;

    List<Place> interestingPlaces = new ArrayList<>();
    int searchRadius;
    int countPlacesUpperBound;

    String currentTemperature;
    String currentWindSpeed;
    String weatherDescription;

    public String getLocationDescription() {
        return "locationName: " +
                locationName +
                "\n\tcounty: " +
                country +
                "\n\tcity: " +
                city +
                "\n\tstreet: " +
                street +
                "\n\tlongitude: " +
                longitude +
                "\n\tlatitude: " +
                latitude;
    }

    public String getCurrentWeather() {
        return "Current location weather:" +
                "\n\tCurrent temperature: " + currentTemperature +
                "\n\tCurrent wind speed: " + currentWindSpeed +
                "\n\tWeather description: " + weatherDescription + "\n";
    }

    public String getInterestingPlacesData() {
        StringBuilder sb = new StringBuilder();
        sb.append("List of interesting places in this location within a radius of ")
                .append(searchRadius)
                .append(" meters with a description:\n");
        for (Place place : interestingPlaces) {
            sb.append('\t');
            sb.append("Place name: ");
            sb.append(place.name);
            sb.append("\n\tXID: ");
            sb.append(place.xid);
            sb.append("\n\tDescription: ");
            sb.append(place.description);
            sb.append("\n\n");
        }
        return sb.toString();
    }

}
