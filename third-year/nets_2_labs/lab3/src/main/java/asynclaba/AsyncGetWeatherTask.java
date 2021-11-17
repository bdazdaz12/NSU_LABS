package asynclaba;

import org.asynchttpclient.AsyncHttpClient;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;


public class AsyncGetWeatherTask implements Runnable {

    private final AsyncHttpClient asyncHttpClientInstance;
    private final Location requiredLocation;

    AsyncGetWeatherTask(AsyncHttpClient asyncHttpClientInstance,
                        Location requiredLocation) {
        this.asyncHttpClientInstance = asyncHttpClientInstance;
        this.requiredLocation = requiredLocation;
    }

    @Override
    public void run() {
        String url = "https://api.openweathermap.org/data/2.5/weather?lat=" + requiredLocation.latitude +
                "&lon=" + requiredLocation.longitude + "&apikey=d9cc4eb99b79ce7cbcd85ca973cb740a";

        asyncHttpClientInstance.prepareGet(url).execute().toCompletableFuture()
                .thenAccept(response -> {

                    JSONObject parsedResponse;
                    try {
                        parsedResponse = (JSONObject) new JSONParser().parse(response.getResponseBody());
                    } catch (ParseException | NumberFormatException e) {
                        e.printStackTrace();
                        System.err.println("\n\tPARSE_ERROR (Weather JSON) !!!\n");
                        return;
                    }

                    JSONArray weatherArray = (JSONArray) (parsedResponse.get("weather"));

                    requiredLocation.currentTemperature =
                            ((JSONObject) parsedResponse.get("main")).get("temp") == null ? null
                                    : ((JSONObject) parsedResponse.get("main")).get("temp").toString();

                    requiredLocation.currentWindSpeed =
                            ((JSONObject) parsedResponse.get("wind")).get("speed") == null ? null
                                    : ((JSONObject) parsedResponse.get("wind")).get("speed").toString();

                    requiredLocation.weatherDescription =
                            ((JSONObject) weatherArray.get(0)).get("description") == null ? null
                                    : ((JSONObject) weatherArray.get(0)).get("description").toString();
                }).join();
    }
}
