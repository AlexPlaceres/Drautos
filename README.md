# Drautos

> [!WARNING]  
> The following information is critical to know when developing for Drautos:

* [Handling Fatal Exceptions](docs/Exceptions.md#fatal-exceptions)

## Documentation

Full documents can be found in the links below.

* [Exception Handling](docs/Exceptions.md)
* [Logging](docs/Logging.md)

## Dependencies

The following libraries are statically linked to Drautos.

| Name                                                  | Reason for inclusion                                   |
|-------------------------------------------------------|--------------------------------------------------------|
| [Spdlog](https://github.com/gabime/spdlog)            | Logging rich messages to console and file              |
| [Cpptrace](https://github.com/jeremy-rifkin/cpptrace) | Retrieving stack traces to assist with troubleshooting |
| [Detours](https://github.com/microsoft/Detours)       | Hooking functions to alter game behaviour              |