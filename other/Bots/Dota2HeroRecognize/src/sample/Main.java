package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        FXMLLoader loader = new FXMLLoader();
        loader.setLocation(getClass().getResource("sample.fxml"));
        Parent root = loader.load();//FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("Распознавание героев");
        primaryStage.setScene(new Scene(root, 241, 82));
        primaryStage.setResizable(false);
        primaryStage.show();
        Controller controller = loader.getController();
        controller.setStage(primaryStage);
        controller.startThreadSocketAndUDP();
    }


    public static void main(String[] args) {
        launch(args);
    }
}
