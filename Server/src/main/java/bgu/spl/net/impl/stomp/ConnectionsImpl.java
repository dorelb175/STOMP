package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionsImpl<T> implements Connections<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> conectionsMap;
    private ConcurrentHashMap<String, LinkedList<Integer>> topicsMap;
    private ConcurrentHashMap<Integer, User> usersMap;
    private AtomicInteger userID;
    private AtomicInteger msgID;

    public ConnectionsImpl() {
        conectionsMap = new ConcurrentHashMap<>(); //map of Connection handler for each client
        topicsMap = new ConcurrentHashMap<>(); //map of topics: for each topic a list of it's subscribers
        usersMap = new ConcurrentHashMap<>(); //map of the users that logged in to the server
        userID = new AtomicInteger(0); //counter for the connectionId
        msgID = new AtomicInteger(1); //counter for the Message-Id
    }

    @Override
    public boolean send(int connectionId, T msg) {
        boolean done = true;
        try {
            //send the msg to the user using it's ConnectionHandler
            if (conectionsMap.get(connectionId) != null) {
                conectionsMap.get(connectionId).send(msg);
            }
        }
        catch (Exception e) { done = false; }
        return done;
    }

    @Override
    public void send(String channel, T msg) {
        //send the msg to all of the users in the channel
        if (topicsMap.get(channel) != null) {
            for(Integer id: topicsMap.get(channel))
                conectionsMap.get(id).send(msg);
        }
    }

    @Override
    public void disconnect(int connectionId) {
        try {
            usersMap.get(connectionId).setConnected(false);
            conectionsMap.remove(connectionId);
        }
        catch (Exception e) { }
    }

    public void disconnect(int connectionId, String receipt) {
        sendReceipt(connectionId, receipt);
        disconnect(connectionId);
    }

    public void addUser(int connectionId, String username, String passcode) {
        //create a new user and add him to the userMap
        User user = new User(connectionId, username, passcode);
        usersMap.put(connectionId, user);

        //login the user
        login(connectionId, username);
    }

    public void login(int connectionId, String username) {
        getUser(username).setConnected(true);

        //send a CONNECTED frame to the client
        Frame response = new Frame("CONNECTED");
        response.addHeader("version", "1.2");
        send(connectionId, (T) response.toString());
    }

    public int addConnection(ConnectionHandler<T> handler) {
        int id = userID.getAndIncrement();
        //create a new connection and put him in the connectionsMap
        conectionsMap.put(id, handler);
        return id;
    }

    public void subscribe(int connectionId, String topicID, String topic, String receipt) {
        //create a new list for the topic
        topicsMap.putIfAbsent(topic, new LinkedList<>());

        if (!topicsMap.get(topic).contains(connectionId)) {
            //add the user to the topic's book club
            topicsMap.get(topic).add(connectionId);

            //add the topic to the user's topic-id map
            usersMap.get(connectionId).addTopic(topicID, topic);

            //send a RECEIPT to the client
            sendReceipt(connectionId, receipt);
        }
    }

    public void unsubscribe(int connectionId, String topicID, String receipt) {
        //get the topic name according to the topicID of the user
        String topic = usersMap.get(connectionId).getId_topics().get(topicID);

        //if someone is subscribed to this topic
        if (topicsMap.get(topic) != null) {
            //if the user is subscribed to this topic
            if(topicsMap.get(topic).contains(connectionId))
                //remove the user from the topic's book club
                topicsMap.get(topic).remove((Integer)connectionId);

            //send a RECEIPT to the client
            sendReceipt(connectionId, receipt);
        }
    }

    public void sendToTopic(String topic, String body) {
        //send MESSAGE to all subscribers of the topic
        sendMessage(topic, body);
    }

    public boolean isConnected(int connectionId) {
        if (usersMap.get(connectionId) == null)
            return false;

        return usersMap.get(connectionId).isConnected();
    }

    public boolean isConnectedByUsername(String username) {
        if (getUser(username) == null)
            return false;

        return getUser(username).isConnected();
    }

    public boolean userExists(String username) {
        return getUser(username) != null;
    }

    private User getUser(String username) {
        User user = null;
        boolean found = false;
        Iterator iter = usersMap.entrySet().iterator();
        //check if this username already exists
        while (!found && iter.hasNext()) {
            Map.Entry<Integer, User> entry = (Map.Entry)iter.next();
            String name = entry.getValue().getName();
            if (name.equals(username)) {
                user = entry.getValue();
                found = true;
            }
        }
        return user;
    }

    public boolean correctPasscode(String username, String passcode) {
        return getUser(username).getPasscode().equals(passcode);
    }

    public void sendError(int connectionId, String msg, String body) {
        //send the ERROR frame to the client
        Frame response = new Frame("ERROR");
        response.addHeader("message", msg);
        response.setBody(body);
        send(connectionId, (T) response.toString());

        //close the connection with the client
        disconnect(connectionId);
    }

    private void sendMessage(String destination, String body) {
        //send the MESSAGE frame to all of the users in the channel
        if (topicsMap.get(destination) != null) {
            for(Integer i: topicsMap.get(destination)) {
                if (usersMap.get(i).isConnected()) {
                    //send the MESSAGE frame to the client
                    Frame response = new Frame("MESSAGE");

                    Integer id = msgID.getAndIncrement();
                    response.addHeader("Message-id", id.toString());
                    String topicID = usersMap.get(i).getIdOfTopic(destination);
                    response.addHeader("subscription", topicID.toString());
                    response.addHeader("destination", destination);

                    response.setBody(body);

                    conectionsMap.get(i).send((T) response.toString());
                }
            }
        }
    }

    private void sendReceipt(int connectionId, String receipt) {
        //send the RECEIPT frame to the client
        Frame response = new Frame("RECEIPT");
        response.addHeader("receipt-id", receipt);
        send(connectionId, (T) response.toString());
    }
}
