package sample;

import javafx.application.Platform;
import javafx.embed.swing.SwingFXUtils;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.image.PixelFormat;
import javafx.scene.image.WritableImage;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.concurrent.CountDownLatch;


public class DrawingController {

    private final Canvas canvas;
    private ImageView imageView;
    private final GraphicsContext graphicsContext;
    private WritableImage writableImage;


    public DrawingController(Canvas canvas, ImageView imageView) {
        this.canvas = canvas;
        this.imageView = imageView;

        this.graphicsContext = canvas.getGraphicsContext2D();

        writableImage = new WritableImage((int)canvas.getWidth(),(int)canvas.getHeight());


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
                    graphicsContext.getCanvas().snapshot(null,writableImage);
                });

        canvas.addEventHandler(MouseEvent.MOUSE_RELEASED,
                event -> {
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

    public Canvas getCanvas(){
        return canvas;
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

    public byte[] getByteArrayFromCanvas(){
        File outputFile = new File("draw.png");
        ByteArrayOutputStream bos = new ByteArrayOutputStream();

        try {
//            ImageIO.write( SwingFXUtils.fromFXImage( writableImage, null ), "png", outputFile );
            ImageIO.write( SwingFXUtils.fromFXImage( writableImage, null ), "png", bos );
        } catch (IOException e) {
            e.printStackTrace();
        }

        return bos.toByteArray();
    }
}
