<?hh
namespace APIs\Applications\RESTAPIs;
use APIs\Domain\Configurations\RESTAPIs\Executables\ExecutableRESTAPI;

interface RESTAPIApplication {
    public function execute(ExecutableRESTAPI $restAPI);
}
