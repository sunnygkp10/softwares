<?hh
namespace APIs\Domain\Configurations\RESTAPIs\Executables;
use APIs\Domain\Configurations\RESTAPIs\RESTAPI;

interface ExecutableRESTAPI {
    public function getRESTAPI(): RESTAPI;
    public function hasParams(): boolean;
    public function getParams(): ?Map<string, string>;
}
