package asynclaba;

import org.asynchttpclient.AsyncHttpClient;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.util.ArrayList;
import java.util.List;

public final class AsyncFindPlacesTask implements Runnable {

    private final AsyncHttpClient asyncHttpClientInstance;
    private final Location requiredLocation;
    private static final String apiKey = "5ae2e3f221c38a28845f05b69e1ef792435c955768d705b9735bd4bc";


    AsyncFindPlacesTask(AsyncHttpClient asyncHttpClientInstance,
                        Location requiredLocation) {
        this.asyncHttpClientInstance = asyncHttpClientInstance;
        this.requiredLocation = requiredLocation;
    }

    @Override
    public void run() {
        String findPlacesUrl = "https://api.opentripmap.com/0.1/ru/places/radius?radius="
                + requiredLocation.searchRadius
                + "&lon=" + requiredLocation.longitude
                + "&lat=" + requiredLocation.latitude
                + "&apikey=" + apiKey;


        asyncHttpClientInstance.prepareGet(findPlacesUrl).execute().toCompletableFuture()
                .thenAccept(response -> {
                    JSONArray parsedResponse;
                    try {
                        parsedResponse = (JSONArray) new JSONParser().parse(response.getResponseBody());
                    } catch (ParseException | NumberFormatException e) {
                        e.printStackTrace();
                        System.err.println("\n\tPARSE_ERROR (Places_1 JSON) !!!\n");
                        return;
                    }

                    for (var place : parsedResponse) {
                        Place nextFoundPlace = new Place();

                        nextFoundPlace.xid = ((JSONObject) place).get("xid").toString();
                        nextFoundPlace.name = ((JSONObject) place).get("name").toString();
                        requiredLocation.interestingPlaces.add(nextFoundPlace);
                    }
                });


//
//        String findPlacesDescUrl = "https://api.opentripmap.com/0.1/ru/places/xid/"
//                + "R"
//                + "?apikey=" + apiKey;
    }
}
