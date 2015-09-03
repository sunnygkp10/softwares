<?hh
namespace APIs\Domain;
use APIs\Domain\Configurations\Routes\Route;

interface API {
    public function getName(): string;
    public function getRoutes(): Vector<Route>;
    public function hasConfigurations(): boolean;
    public function getConfigurations(): Map<string, string>;
    public function hasEnvironmentVariables(): boolean;
    public function getEnvironmentVariables(): ?Map<string, string>;
    public function hasHttpRequests(): boolean;
    public function getHttpRequests(): ?Map<string, string>;
    public function hasCLI(): boolean;
    public function getCLI(): ?Map<string, string>;
}
