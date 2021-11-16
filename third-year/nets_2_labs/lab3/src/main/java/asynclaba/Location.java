package asynclaba;


import java.util.List;

public final class Location {

    String latitude;
    String longitude;
    String locationName;
    String country;
    String city;
    String street;

    List<Place> interestingPlaces = null;
    int searchRadius;

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
                "\n\t" +  currentTemperature +
                "\n\t" + currentWindSpeed +
                "\n\t" + weatherDescription + "\n";
    }

//    public List<Place> getInterestingPlaces() {
//        StringBuilder sb = new StringBuilder();
//        sb.append("list of interesting places in this location\n within a radius of ")
//                .append(searchRadius)
//                .append(" meters with a description:\n");
//        for (Place place : interestingPlaces) {
//            sb.append('\t');
//            sb.append(place.name);
//            sb.append("\n\n");
//        }
//        return sb.toString();
//    }


    public String getInterestingPlacesData() {
        StringBuilder sb = new StringBuilder();
        sb.append("list of interesting places in this location\n within a radius of ")
                .append(searchRadius)
                .append(" meters with a description:\n");
        for (Place place : interestingPlaces) {
            sb.append('\t');
            sb.append(place.name);
            sb.append("\n\n");
        }
        return sb.toString();
    }

}
