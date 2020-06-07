package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class Frame {
    private final String command;
    private HashMap<String, String> headers;
    private String body;

    public Frame(String command, HashMap<String, String> headers, String body) {
        this.command = command;
        this.headers = headers;
        this.body = body;
    }

    public Frame(String command) {
        this(command, new HashMap<>(), "");
    }

    public String getCommand() {
        return command;
    }

    public Map<String, String> getHeaders() {
        return headers;
    }

    public void addHeader(String header, String value) {
        headers.put(header, value);
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    @Override
    public String toString() {
        StringBuffer string = new StringBuffer(command);
        string.append( "\n" );

        if (headers != null) {
            for (Iterator keys = headers.keySet().iterator(); keys.hasNext();) {
                String key = (String)keys.next();
                String value = (String)headers.get(key);
                string.append( key );
                string.append( ":" );
                string.append( value );
                string.append( "\n" );
            }
        }
        string.append( "\n" );

        if (body.compareTo("") != 0)
            string.append( body );

        return string.toString();
    }

    public static Frame toFrame(String str) {
        String[] parts = str.split("\n");
        String command = parts[0];
        HashMap<String, String> _headers = new HashMap<>();
        String body = "";

        //SEND frame always have body
        if (command.compareTo("SEND") == 0) {
            //Body:
            int bodyIndex = -1;
            for (int i = (int) parts.length - 1; bodyIndex == -1 && i > 0; i--) {
                if (parts[i].compareTo("") != 0) {
                    bodyIndex = i;
                    body = parts[i];
                }
            }

            boolean empty = false;
            //headers:
            for (int i = 1; !empty && i < bodyIndex; i++) {
                if (parts[i].compareTo("") != 0) {
                    String[] headerParts = parts[i].split(":");
                    _headers.put(headerParts[0], headerParts[1]);
                }
                else
                    empty = true;
            }
        }
        else {
            //headers:
            boolean empty = false;
            for (int i = 1; !empty && i < (int) parts.length; i++) {
                if (parts[i].compareTo("") != 0) {
                    String[] headerParts = parts[i].split(":");
                    _headers.put(headerParts[0], headerParts[1]);
                }
                else
                    empty = true;
            }
        }

        return new Frame(command, _headers, body);
    }
}

