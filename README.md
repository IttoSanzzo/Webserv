<h1 align="center">C++ Web Server</h1>

<p align="center"><img width=500px alt="Example" src="https://i.imgur.com/Z2c2ggy.png"></p>

<p align="center">
<img alt="License" src="https://img.shields.io/badge/license-MIT-blue.svg">
<img alt="C++" src="https://img.shields.io/badge/c++-%2300599C.svg?logo=c%2B%2B&logoColor=white">
</p>

## About It

-   I created this pretty much just for fun, and it was handmade practically from scratch.
-   Naturally, since doing it using an easier language, such as C#, wouldn't be any fun, and also doing it using modern C++ and external libraries would make the process rather trivial, I imposed some limitations on myself.
-   For the sake of learning, practicing and, above all, having fun, I decided to follow the C++98 standard and use almost only the C++ std functions, so all the mechanisms and functions here were made by me, including the [Json Engine](https://github.com/IttoSanzzo/CppJsonHandler), with the exception of functions already built into C++98 itself, or needed some direct access to the Kernel, and would not be efficient to recreate them.
-   I also made it single thread, so that it would be more difficult and at the same time, necessary, to squeeze the maximum performance out of it.
-   But even while being single threaded, it can actually serve multiple different combinations of hosts and ports, each with their own set of configurations, all at the same time.
-   This Web Server implements basically all that's needed in a modern WebServer, with support to the most common http1.1 methods, like GET, POST and DELETE, handles the correct response codes for each transaction, can handle cookies, and also suports CGI.
-   For that, it ended up costing me almost an entire month in days to wrap-up a satisfactory version of it.
-   Obliviously, it does not leak memory, and I was not able to break it in none of my... rather simple tests, and common use, till now.
-   But also, because I made _fast results_ a must, while keeping total modularity, it ended up maybe not being the best tool performance or memory wise. BUT, it does attend my demands for the end it was made, flawlessly, and that's what matter the most, as it's still pretty much _REALLY FAST_ for a single thread WebServer.
-   And well, I said that, but I'm pretty sure it won't bring any usage problem while not being used to pretty heavily massive ends.

## Compiling

-   First, just clone it.
-   Then use `git submodule update --init --recursive` inside the cloned repository. This will initiate the [Json Engine](https://github.com/IttoSanzzo/CppJsonHandler) sub-repo.
-   Now use `make`. This will compile the files into a `webserv` file.

## How to use it

<p align="center"><img width=500px alt="Default Config" src="https://i.imgur.com/KiKU5k7.png"></p>

-   You can run it using `./webserv`, and it will user the default configuration file located at `./configs/default.json`. You can modify the default configuration to your needs, or alternatively, you can run `./webserv "./path/to/config"` to use a expecific configuration file.
-   It comes with some pre-made configuration files, and mocked pages, to exemplify how it works.
-   It will accept any valid json file that has the minimal information to run a server, which are a valid port, a valid host, and some routes.
-   Other aspects can be tweaked as shown in the example image above.
-   From that, as it's running, it will show the current imported configs in the terminal, open at the given host:port, and that will be available in your pc, for your browser and other access means.

<p align="center"><img width=500px alt="Default Config" src="https://i.imgur.com/dE8HZB0.png"></p>

-   To access it from outside, you will need to set a port forwaring in your router, to access in your local internet, and open your firewalls to connect from World Wide.

## Capabilities

-   As said before, this Web Server can handle the more common http1.1 methods like GET, POST and DELETE normally. That function can be restrained invidually to each route with the configuration file.
-   Any configured route's access permission are recursive to it's sub-routes.
-   For directory routes, you can set auto-indexing, to send a simple automatic, static and interactive page to the browser, which will show all files and folders inside the accessed routes index, or path.

<p align="center"><img alt="Auto-indexing Example" src="https://i.imgur.com/KY6SA1K.png"></p>

-   All paths and routes accessed will be concatenated to the "dataDirectory" configuration. If there's none, then it will be the root of your WebServer.
-   You can too set routes to redirect to other routes or links, in a routes configuration.
-   Setting a route to use cgi, will make GETs to it, run the given file in the configuration, with the used query string, and cookies. You need to set the relations between a "CGI" file extension, and it's executable. Like bash for .sh, and python3 for python

## Pure Power

-   While accessed without Keep Alive, it can serve at least 255 clients at the same time with ease, realizing at least 19000 hits, with latency between 0.01 and 0.69 second with no error, constantly for a infinite ammount of time.

<p align="center"><img alt="Example without Keep Alive" src="https://i.imgur.com/Qhm5r1V.png"></p>

-   But if accessed with Keep Alive, it can serve the same 255 clients at the same time with much more ease, realizing at around 50000 hits, with latency between 0.00 and 0.70 second with no error, for the same ammount of time.

<p align="center"><img alt="Example with Keep Alive" src="https://i.imgur.com/9l4ajcv.png"></p>

## Error Handling

-   While there is an automatic error handling, that serves static pages for each error code, you can set your own error pages, by inserting them in the configuration file.

<p align="center"><img alt="Error 404 example" src="https://i.imgur.com/cAVExjw.png"></p>

---

<p align="center">Done with lots of coffee,💜, and C++, by <a href="https://github.com/IttoSanzzo">IttoSanzzo</a>.</p>
