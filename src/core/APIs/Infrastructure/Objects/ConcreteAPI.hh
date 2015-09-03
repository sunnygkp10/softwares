<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\API;
use APIs\Domain\Configurations\Routes\Route;

final class ConcreteAPI implements API {
    private $name;
    private $routes;
    private $environmentVariables;
    private $httpRequests;
    private $cli;
    private $configurations;
    public function __construct(string $name, Vector<Route> $routes, Map<string, string> $environmentVariables = null, Map<string, string> $httpRequests = null, Map<string, string> $cli = null, Map<string, string> $configurations = null) {

        if (empty($name)) {
            throw new \Exception('The name of the API cannot be empty.');
        }

        if (empty($environmentVariables)) {
            $environmentVariables = null;
        }

        if (empty($httpRequests)) {
            $httpRequests = null;
        }

        if (empty($configurations)) {
            $configurations = null;
        }

        $this->name = $name;
        $this->routes = $routes;
        $this->environmentVariables = $environmentVariables;
        $this->httpRequests = $httpRequests;
        $this->cli = $cli;
        $this->configurations = $configurations;
    }

    public function getName(): string {
        return $this->name;
    }

    public function getRoutes(): Vector<Route> {
        return $this->routes;
    }

    public function hasConfigurations(): boolean {
        return !empty($this->configurations);
    }

    public function getConfigurations(): Map<string, string> {
        return $this->configurations;
    }

    public function hasEnvironmentVariables(): boolean {
        return !empty($this->environmentVariables);
    }

    public function getEnvironmentVariables(): ?Map<string, string> {
        return $this->environmentVariables;
    }

    public function hasHttpRequests(): boolean {
        return !empty($this->httpRequests);
    }

    public function getHttpRequests(): ?Map<string, string> {
        return $this->httpRequests;
    }

    public function hasCLI(): boolean {
        return !empty($this->cli);
    }

    public function getCLI(): ?Map<string, string> {
        return $this->cli;
    }

}
