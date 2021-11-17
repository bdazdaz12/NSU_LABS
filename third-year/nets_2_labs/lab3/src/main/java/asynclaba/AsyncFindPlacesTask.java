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

public final class AsyncFindPlacesTask implements Runnable {

    private final AsyncHttpClient asyncHttpClientInstance;
    private final Location requiredLocation;
    private static final String apiKey = "5ae2e3f221c38a28845f05b69e1ef792435c955768d705b9735bd4bc";


    AsyncFindPlacesTask(AsyncHttpClient asyncHttpClientInstance,
                        Location requiredLocation) {
        this.asyncHttpClientInstance = asyncHttpClientInstance;
        this.requiredLocation = requiredLocation;
    }

    private CompletableFuture<List<Place>> findPlacesByXid() {
        String findPlacesUrl = "https://api.opentripmap.com/0.1/ru/places/radius?radius="
                + requiredLocation.searchRadius
                + "&lon=" + requiredLocation.longitude
                + "&lat=" + requiredLocation.latitude
                + "&format=json"
                + "&apikey=" + apiKey;


        return asyncHttpClientInstance.prepareGet(
                findPlacesUrl).execute()
                .toCompletableFuture()
                .thenApply(response -> { // ищем места по coords
                    JSONArray parsedResponse = null;
                    try {
                        parsedResponse = (JSONArray) new JSONParser().parse(response.getResponseBody());
                    } catch (ParseException e) {
                        e.printStackTrace();
                        System.err.println("\n\tPARSE_ERROR (Places_1 JSON) !!!\n");
                        System.exit(23);
                    }

                    List<Place> places = new ArrayList<>();
                    int countPLaces = 0;
                    for (var place : parsedResponse) {
                        if (countPLaces >= requiredLocation.countPlacesUpperBound) {
                            break;
                        }
                        countPLaces++;

                        Place nextFoundPlace = new Place();
                        nextFoundPlace.xid = ((JSONObject) place).get("xid").toString();
                        nextFoundPlace.name = ((JSONObject) place).get("name").toString();

                        if (nextFoundPlace.name.isEmpty()) {
                            continue;
                        }
                        places.add(nextFoundPlace);
                    }
                    return places;
                });
    }

    private void findPlacesDescriptions(List<Place> foundPlaces) {
        String urlPrefix = "https://api.opentripmap.com/0.1/ru/places/xid/";
        String urlPostfix = "?apikey=" + apiKey;
        List<CompletableFuture<Void>> findPlacesDescriptionsAsyncRequests = new ArrayList<>();

        for (Place place : foundPlaces) {
            findPlacesDescriptionsAsyncRequests.add(asyncHttpClientInstance
                    .prepareGet(urlPrefix + place.xid + urlPostfix)
                    .execute()
                    .toCompletableFuture()
                    .thenAcceptAsync(response -> {
                        JSONObject parsedResponse;
                        try {
                            parsedResponse = (JSONObject) new JSONParser()
                                    .parse(response.getResponseBody());
                        } catch (ParseException e) {
                            e.printStackTrace();
                            System.err.println("\n\tPARSE_ERROR (Places_2 JSON) !!!\n");
                            return;
                        }

                        if (parsedResponse.get("wikipedia_extracts") != null
                                && ((JSONObject) parsedResponse.get("wikipedia_extracts")).get("text") != null) {
                            place.description = ((JSONObject) parsedResponse.get("wikipedia_extracts"))
                                    .get("text").toString();
                        }
                    }));
        }
        CompletableFuture.allOf(findPlacesDescriptionsAsyncRequests.toArray(new CompletableFuture[0])).join();
    }

    @Override
    public void run() {
        List<Place> foundPlaces = findPlacesByXid().join();
        findPlacesDescriptions(foundPlaces);
        requiredLocation.interestingPlaces = foundPlaces;
    }
}
