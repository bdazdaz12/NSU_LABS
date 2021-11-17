package asynclaba;

import org.apache.log4j.PropertyConfigurator;
import org.asynchttpclient.AsyncHttpClient;
import org.asynchttpclient.Dsl;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

public class Main {

    public static void main(String[] args) {
        String log4jConfPath = "src/main/resources/log4j.properties";
        PropertyConfigurator.configure(log4jConfPath); // for want?

        Scanner sc = new Scanner(System.in);

        try (AsyncHttpClient asyncHttpClientInstance = Dsl.asyncHttpClient()) {
            System.out.print("Введите название искомой локации: ");
            String searchingLocationName = sc.nextLine();

            Future<Location> locationFuture = CompletableFuture
                    .supplyAsync(new AsyncLocationsSupplier(asyncHttpClientInstance, searchingLocationName))
                    .thenApply(foundLocations -> {
                        System.out.println("Список найденных по вашему запросу мест:");
                        System.out.println("\n----------------------------------------------------");
                        int idx = 1;
                        for (Location location : foundLocations) {
                            System.out.println("Номер локации: " + idx);
                            ++idx;
                            System.out.println(location.getLocationDescription());
                        }

                        System.out.print("\nВведите номер нужной вам локации: ");
                        int requiredLocationNumber = sc.nextInt() - 1;

                        System.out.print("Введите радиус поиска интересных мест (в метрах): ");
                        foundLocations.get(requiredLocationNumber).searchRadius = sc.nextInt();

                        System.out.print("Введите максимальное количество интересных мест: ");
                        foundLocations.get(requiredLocationNumber).countPlacesUpperBound = sc.nextInt();

                        System.out.println("----------------------------------------------------\n");

                        return foundLocations.get(requiredLocationNumber);
                    })
                    .thenApply(requiredLocation -> {
                        List<CompletableFuture<Void>> tasks = new ArrayList<>();

                        tasks.add(CompletableFuture
                                .runAsync(new AsyncGetWeatherTask(asyncHttpClientInstance, requiredLocation)));
                        tasks.add(CompletableFuture
                                .runAsync(new AsyncFindPlacesTask(asyncHttpClientInstance, requiredLocation)));

                        CompletableFuture.allOf(tasks.toArray(new CompletableFuture[0])).join();

                        return requiredLocation;
                    });


            Location foundLocationData = locationFuture.get();

            System.out.println(foundLocationData.getCurrentWeather());
            System.out.println(foundLocationData.getInterestingPlacesData());
        } catch (IOException | ExecutionException | InterruptedException e) {
            e.printStackTrace();
        }

    }
}
