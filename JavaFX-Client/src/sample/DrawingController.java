package sample;

import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.image.PixelFormat;
import javafx.scene.image.PixelWriter;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.LineTo;
import javafx.scene.shape.MoveTo;
import javafx.scene.shape.Path;

import java.nio.ByteBuffer;
import java.util.Random;

public class DrawingController {

    private final Canvas canvas;
    private ImageView imageView;
    private final GraphicsContext graphicsContext;
    private Path path;

    public DrawingController(Canvas canvas, ImageView imageView) {
        this.canvas = canvas;
        this.imageView = imageView;

        this.graphicsContext = canvas.getGraphicsContext2D();

        initDraw(graphicsContext);

        canvas.addEventFilter(MouseEvent.ANY, (e) -> canvas.requestFocus());

        canvas.addEventHandler(MouseEvent.MOUSE_PRESSED,
                event -> {
                    graphicsContext.beginPath();
                    graphicsContext.moveTo(event.getX(), event.getY());
                    graphicsContext.stroke();
                });

        canvas.addEventHandler(MouseEvent.MOUSE_DRAGGED,
                event -> {
                    graphicsContext.lineTo(event.getX(), event.getY());
                    graphicsContext.stroke();
                });

        canvas.addEventHandler(MouseEvent.MOUSE_RELEASED,
                event -> {
                });
        canvas.addEventHandler(KeyEvent.KEY_PRESSED,
                event -> {
                    colorPoint((new Random()).nextInt((int)canvas.getWidth()) , (new Random()).nextInt((int)canvas.getHeight()));
                });
    }

    private void initDraw(GraphicsContext gc) {
        double canvasWidth = gc.getCanvas().getWidth();
        double canvasHeight = gc.getCanvas().getHeight();

        gc.setFill(Color.LIGHTGRAY);
        gc.setStroke(Color.BLACK);
        gc.setLineWidth(5);

        gc.fill();
        gc.strokeRect(
                0,              //x of the upper left corner
                0,              //y of the upper left corner
                canvasWidth,    //width of the rectangle
                canvasHeight);  //height of the rectangle

        gc.setFill(Color.RED);
        gc.setStroke(Color.BLACK);
        gc.setLineWidth(1);
    }


    private void colorPoint(int x, int y) {
          graphicsContext.moveTo(x,y);
          graphicsContext.lineTo(x,y);
          graphicsContext.stroke();
    }

    public void updateImage(Image image) {
        canvas.setVisible(false);
        imageView.setImage(image);
    }

    public void allowDrawing(boolean drawing) {
        imageView.setVisible(!drawing);
        canvas.setVisible(drawing);
    }
}
