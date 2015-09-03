<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\RESTAPIs\Executables\ExecutableRESTAPI;
use APIs\Domain\Configurations\RESTAPIs\RESTAPI;

final class ConcreteExecutableRESTAPI implements ExecutableRESTAPI {
    private $restAPI;
    private $params;
    public function __construct(RESTAPI $restAPI, Map<string, string> $params = null) {

        if (empty($params)) {
            $params = null;
        }

        $this->restAPI = $restAPI;
        $this->params = $params;

    }

    public function getRESTAPI(): RESTAPI {
        return $this->restAPI;
    }

    public function hasParams(): boolean {
        return !empty($this->params);
    }

    public function getParams(): ?Map<string, string> {
        return $this->params;
    }

}
