package sample;

import javafx.application.Platform;
import javafx.embed.swing.SwingFXUtils;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.image.WritableImage;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;

import javax.imageio.ImageIO;
import java.io.ByteArrayOutputStream;
import java.io.IOException;


public class DrawingController {

    private final Canvas canvas;
    private ImageView imageView;
    private final GraphicsContext graphicsContext;
    private WritableImage writableImage;
    private boolean changed;


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
                    changed = true;
                });

        canvas.addEventHandler(MouseEvent.MOUSE_RELEASED,
                event -> {
                    changed = true;
                });

    }

    private void initDraw(GraphicsContext gc) {
        gc.setStroke(Color.BLACK);
        gc.setLineWidth(1);
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
        ByteArrayOutputStream bos = new ByteArrayOutputStream();

        try {
            ImageIO.write( SwingFXUtils.fromFXImage( writableImage, null ), "png", bos );
        } catch (IOException e) {
            e.printStackTrace();
        }

        return bos.toByteArray();
    }

    public void clearImage(){
        imageView.setImage(null);
        graphicsContext.clearRect(0,0,canvas.getWidth(),canvas.getHeight());

        Platform.runLater(()->{
            graphicsContext.getCanvas().snapshot(null,writableImage);
        });
    }

    public boolean imageHasChanged() {
        return changed;
    }

    public void setHasChanged(boolean changed) {
        this.changed = changed;
    }
}
