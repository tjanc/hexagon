//  Copyright 2018 Thomas Jandecka.
//  Subject to GNU GENERAL PUBLIC LICENSE Version 3.

#include <boost/filesystem.hpp>

#include "listener.hpp"
#include "net.hpp"
#include "shared_state.hpp"
#include "websocket_session.hpp"

#include "preload_assets.hpp"

#include <boost/program_options.hpp>

int main(int argc, const char* argv[])
{
    using namespace hexagon::server;
    using boost::system::error_code;
    namespace po = boost::program_options;

    const auto assets_path = boost::filesystem::path(argv[0])
                                 .remove_filename()  //
                                 .parent_path()      //
                                 .append("lib")      //
                                 .append("hexagon")  //
                                 .append("assets");

    po::options_description param_desc("Usage");
    param_desc.add_options()                                             //
        ("help",                                                         //
         "print this help message")                                      //
        ("port,p",                                                       //
         po::value<unsigned short>()->default_value(8080),               //
         "port number")                                                  //
        ("documents,d",                                                  //
         po::value<std::string>(),                                       //
         "path to public document directory")                            //
        ("assets,a",                                                     //
         po::value<std::string>()->default_value(assets_path.string()),  //
         "path to assets directory")                                     //
        ;

    po::variables_map params;
    po::store(po::parse_command_line(argc, argv, param_desc), params);
    po::notify(params);

    if (params.count("help")) {
        std::cout << param_desc << '\n';
        return EXIT_SUCCESS;
    }

    if (!params.count("port")) {
        std::cerr << "Missing port\n";
        std::cerr << param_desc << '\n';
        return EXIT_FAILURE;
    }
    const auto port = params["port"].as<unsigned short>();

    if (!params.count("documents")) {
        std::cerr << "Missing public document directory\n";
        std::cerr << param_desc << '\n';
        return EXIT_FAILURE;
    }
    const auto documents_path = params["documents"].as<std::string>();

    if (!params.count("assets")) {
        std::cerr << "Missing assets directory\n";
        std::cerr << param_desc << '\n';
        return EXIT_FAILURE;
    }
    preload_assets assets{params["assets"].as<std::string>()};

    net::io_context ioc;
    {
        auto endpoint = tcp::endpoint{tcp::v4(), port};
        auto l = std::make_shared<listener>(
            ioc, std::move(endpoint),
            std::make_shared<shared_state>(std::move(assets), documents_path));

        l->run();
    }

    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](const error_code&, int) { ioc.stop(); });

    ioc.run();

    return EXIT_SUCCESS;
}
