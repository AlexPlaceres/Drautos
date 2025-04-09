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

## Deployment

`win-x64` releases of `Drautos` are released automatically via NuGet when running the workflow defined by
[main.yml](.github/workflows/main.yml) through GitHub Actions. This is currently set to be triggered manually.

> [!IMPORTANT]  
> Version number must be updated manually in [Drautos.nuspec](Drautos.nuspec) prior to running the deployment workflow.
