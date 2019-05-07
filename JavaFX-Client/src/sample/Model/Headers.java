package sample.Model;

import java.util.HashMap;

public class Headers {
    public static HashMap<String, HeaderType> HEADERS = new HashMap<String, HeaderType>() {{
        put("CHAT_MESSAGE", HeaderType.CHAT_MESSAGE);
        put("WELCOME_USER", HeaderType.WELCOME_USER);
        put("HELLO_SERVER", HeaderType.HELLO_SERVER);
        put("SEND_MESSAGE", HeaderType.SEND_MESSAGE);
        put("UPDATECANVAS", HeaderType.UPDATECANVAS);
        put("GAME___ENDED", HeaderType.GAME_ENDED);
        put("GAME_WAITING", HeaderType.GAME_WAITING);
        put("GAME___READY", HeaderType.GAME_READY);
        put("YOUAREDRAWER", HeaderType.YOU_ARE_DRAWER);
        put("CLUEINCORECT", HeaderType.CLUE_INCORRECT);
        put("CLUE_CORRECT", HeaderType.CLUE_CORRECT);
    }};
}
