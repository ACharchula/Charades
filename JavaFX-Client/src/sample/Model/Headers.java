package sample.Model;

import java.util.HashMap;

public class Headers {
    public static HashMap<String, HeaderType> HEADERS = new HashMap<String, HeaderType>() {{
        put("CHAT_MESSAGE", HeaderType.CHAT_MESSAGE);
        put("WELCOME_USER", HeaderType.WELCOME_USER);
        put("HELLO_SERVER", HeaderType.HELLO_SERVER);
        put("SEND_MESSAGE", HeaderType.SEND_MESSAGE);
    }};
}
