package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;


public class User {
    private final int connectionID;
    private final String name;
    private final String passcode;
    private HashMap<String, String> id_topics;
//    private HashMap<String, LinkedList<String>> inventory;
    private boolean isConnected;

    public User(int connectionID, String name, String passcode) {
        this.connectionID = connectionID;
        this.name = name;
        this.passcode = passcode;
        id_topics = new HashMap<>();
        isConnected = false;
    }

    public int getConnectionID() {
        return connectionID;
    }

    public String getName() {
        return name;
    }

    public String getPasscode() {
        return passcode;
    }

    public HashMap<String, String> getId_topics() {
        return id_topics;
    }

    public String getIdOfTopic(String topic) {
        String i = "-1";
        Iterator iter = id_topics.entrySet().iterator();
        while (iter.hasNext()) {
            Map.Entry<String, String> entry = (Map.Entry)iter.next();
            if(topic.equals(entry.getValue()))
                i = entry.getKey();
        }
        return i;
    }


    public void addTopic(String topicID, String topic) {
        id_topics.put(topicID, topic);
    }

    public void setConnected(boolean connected) {
        isConnected = connected;
    }

    public boolean isConnected() {
        return isConnected;
    }
}
