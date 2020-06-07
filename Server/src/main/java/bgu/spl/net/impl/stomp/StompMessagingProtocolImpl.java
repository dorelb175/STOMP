package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;


public class StompMessagingProtocolImpl implements StompMessagingProtocol {
    private boolean shouldTerminate = false;
    private int connectionId;
    private ConnectionsImpl<String> connections;

    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = (ConnectionsImpl<String>) connections;
    }

    @Override
    public void process(String message) {
        Frame frame = Frame.toFrame(message);
        String command = frame.getCommand();

//        System.out.println("The message received: ");
//        System.out.println(message + "\n");

        if (command.compareTo("CONNECT") == 0) {
            String username = frame.getHeaders().get("login");
            String passcode = frame.getHeaders().get("passcode");

            //If the User doesn't exist - create a new one and change it to logged in
            if (!connections.userExists(username)) {
                connections.addUser(connectionId, username, passcode);
            }
            //The user already exists
            else {
                //Wrong password
                if (!connections.correctPasscode(username, passcode)) {
                    connections.sendError(connectionId, "login failed", "The password that was given doesn't match to the username");
                }
                else {
                    //User is already logged in
                    if (connections.isConnectedByUsername(username)) {
                        connections.sendError(connectionId, "login failed", "User already logged in");
                    }
                    else {
                        connections.login(connectionId, username);
                    }
                }
            }
        }
        else if (command.compareTo("SUBSCRIBE") == 0) {
            if (connections.isConnected(connectionId)) {
                String destination = frame.getHeaders().get("destination");
                String topicID = frame.getHeaders().get("id");
                String receipt = frame.getHeaders().get("receipt");
                connections.subscribe(connectionId, topicID, destination, receipt);
            }
        }
        else if (command.compareTo("UNSUBSCRIBE") == 0) {
            if (connections.isConnected(connectionId)) {
                String topicID = frame.getHeaders().get("id");
                String receipt = frame.getHeaders().get("receipt");
                connections.unsubscribe(connectionId, topicID, receipt);
            }
        }
        else if (command.compareTo("SEND") == 0) {
            String destination = frame.getHeaders().get("destination");
            String body = frame.getBody();
            connections.sendToTopic(destination, body);
        }
        else if (command.compareTo("DISCONNECT") == 0) {
            if (connections.isConnected(connectionId)) {
                String receipt = frame.getHeaders().get("receipt");
                connections.disconnect(connectionId, receipt);
                shouldTerminate = true; //TODO: ????
            }
        }
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}