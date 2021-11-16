package asynclaba;

import org.asynchttpclient.AsyncHttpClient;
import org.asynchttpclient.Response;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.function.Supplier;

public class AsyncLocationsSupplier implements Supplier<List<Location>> {

    private final AsyncHttpClient asyncHttpClientInstance;
    private final String searchingLocationName;

    public AsyncLocationsSupplier(AsyncHttpClient asyncHttpClientInstance,
                                  String searchingLocationName) {
        this.asyncHttpClientInstance = asyncHttpClientInstance;
        this.searchingLocationName = searchingLocationName;
    }

    @Override
    public List<Location> get() {
        String url = "https://graphhopper.com/api/1/geocode?q=" + searchingLocationName
                + "&locale=en&key=21e4d1d9-226e-45ec-b038-ab0405844186";

        CompletableFuture<Response> getResponse = asyncHttpClientInstance.prepareGet(url).execute()
                .toCompletableFuture();

        try {
            List<Location> foundLocations = getResponse.thenApply(response -> {
                JSONObject parsedResponse;
                try {
                    parsedResponse = (JSONObject) new JSONParser().parse(response.getResponseBody());
                } catch (ParseException e) {
                    e.printStackTrace();
                    System.err.println("\n\tPARSE_ERROR (Locations JSON)!!!\n");
                    return null;
                }

                JSONArray hits = (JSONArray) parsedResponse.get("hits");
                ArrayList<Location> locations = new ArrayList<>();

                for (var point : hits) {
                    Location nextFoundLocation = new Location();

                    nextFoundLocation.latitude =
                            ((JSONObject) ((JSONObject) point).get("point")).get("lat").toString();
                    nextFoundLocation.longitude =
                            ((JSONObject) ((JSONObject) point).get("point")).get("lng").toString();
                    nextFoundLocation.locationName =
                            ((JSONObject) point).get("name").toString();

                    nextFoundLocation.city = ((JSONObject) point).get("city") == null ? null
                            : ((JSONObject) point).get("city").toString();

                    nextFoundLocation.street = ((JSONObject) point).get("street") == null ? null
                            : ((JSONObject) point).get("street").toString();

                    nextFoundLocation.country = ((JSONObject) point).get("country") == null ? null
                            : ((JSONObject) point).get("country").toString();

                    locations.add(nextFoundLocation);
                }

                return locations;

            }).get();

            return foundLocations;
        } catch (ExecutionException | InterruptedException e) {
            e.printStackTrace();
            return null;
        }
    }
}
