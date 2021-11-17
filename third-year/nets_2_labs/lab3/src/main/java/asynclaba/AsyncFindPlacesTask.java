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

    @Override
    public void run() {
        String findPlacesUrl = "https://api.opentripmap.com/0.1/ru/places/radius?radius="
                + requiredLocation.searchRadius
                + "&lon=" + requiredLocation.longitude
                + "&lat=" + requiredLocation.latitude
                + "&format=json"
                + "&apikey=" + apiKey;

        asyncHttpClientInstance.prepareGet(
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

                    List<Place> foundPlaces = new ArrayList<>();
                    int countPLaces = 0;
                    for (var place : parsedResponse) {
                        if (countPLaces >= requiredLocation.countPlacesUpperBound) {
                            System.err.println(requiredLocation.countPlacesUpperBound);
                            break;
                        }

                        Place nextFoundPlace = new Place();
                        nextFoundPlace.xid = ((JSONObject) place).get("xid").toString();
                        nextFoundPlace.name = ((JSONObject) place).get("name").toString();

                        if (nextFoundPlace.name.isEmpty()) {
                            continue;
                        }
                        foundPlaces.add(nextFoundPlace);
                        countPLaces++;
                    }
                    return foundPlaces;
                })
                .thenAccept(foundPlaces -> { // ищем описания мест по XID

                    String urlPrefix = "https://api.opentripmap.com/0.1/ru/places/xid/";
                    String urlPostfix = "?apikey=" + apiKey;
                    List<CompletableFuture<Response>> findPlacesDescriptionsAsyncRequests = new ArrayList<>();

                    for (Place place : foundPlaces) {
                        findPlacesDescriptionsAsyncRequests.add(asyncHttpClientInstance
                                .prepareGet(urlPrefix + place.xid + urlPostfix)
                                .execute()
                                .toCompletableFuture());
                    }

                    System.err.println("\nждем завершения ответов " + findPlacesDescriptionsAsyncRequests.size()
                    + "\n");

                    CompletableFuture.allOf(findPlacesDescriptionsAsyncRequests
                            .toArray(new CompletableFuture[0])).join();
                    requiredLocation.interestingPlaces = foundPlaces;



//                    .thenAcceptAsync(response -> {
//                        JSONObject parsedResponse = null;
//
//                        System.err.println("cyka eto otvet daaaa!!!");
//
//                        try {
//                            parsedResponse = (JSONObject) new JSONParser()
//                                    .parse(response.getResponseBody());
//                        } catch (ParseException e) {
//                            e.printStackTrace();
//                            System.err.println("\n\tPARSE_ERROR (Places_2 JSON) !!!\n");
//                            System.exit(23);
//                        }
//
//                        place.description =
//                                ((JSONObject) parsedResponse.get("info")).get("descr") == null ? null
//                                        : ((JSONObject) parsedResponse
//                                        .get("info")).get("descr").toString();
//                    })
                }).join();
    }
}
