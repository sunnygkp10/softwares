<?hh
namespace APIs\Domain\Configurations\Routes\CLI;
use APIs\Domain\Configurations\Variables\Variable;

interface CLIRoute {
    public function hasMandatoryParams(): boolean;
    public function getMandatoryParams(): Map<string, Variable>;
    public function hasOptionalParams(): boolean;
    public function getOptionalParams(): VMap<string, Variable>;
}
