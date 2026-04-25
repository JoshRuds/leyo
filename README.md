# leyo

Leyo is a fully obserbant, permissive language - logs everything you do and makes sure you trust it by having typesafe, allowed code to run only. It's syntax is similar to C, with simplifyed arrays on top (and NO pointers!!!).

EXAMPLE:

allow NetworkInbound(port:5000);
defined networkWatch(networkTraffic: NetworkStream) {
    log 'watching stream on #port#';
    *** comment;
    if (port === 5000) {
        pass;
    };
};