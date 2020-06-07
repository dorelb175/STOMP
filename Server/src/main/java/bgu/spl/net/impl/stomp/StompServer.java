package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        int port = Integer.parseInt(args[0]);
        String type = args[1];

        if (type.compareTo("tpc") == 0)
            Server.threadPerClient(port, StompMessagingProtocolImpl::new, StompEncoderDecoder::new).serve();
        else if (type.compareTo("reactor") == 0)
            Server.reactor(Runtime.getRuntime().availableProcessors(), port, StompMessagingProtocolImpl::new, StompEncoderDecoder::new).serve();
    }
}
