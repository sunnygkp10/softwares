<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Routes\CLI\CLIRoute;
use APIs\Domain\Configurations\Variables\Variable;

final class ConcreteCLIRoute implements CLIRoute {
    private $mandatoryParams;
    private $optionalPams;
    public function __construct(Map<string, Variable> $mandatoryParams = null, Map<string, Variable> $optionalPams = null) {

        if (empty($mandatoryParams)) {
            $mandatoryParams = null;
        }

        if (empty($optionalPams)) {
            $optionalPams = null;
        }

        if (!empty($mandatoryParams) && !empty($optionalPams)) {
            $values = $mandatoryParams->values();
            foreach($values as $oneVariable) {
                if (in_array($oneVariable, $optionalPams)) {
                    throw new \Exception('The param ('.$oneVariable->getName().') is present in both mandatory and optional params.  Each param can only be present in one or the pther.  Not both.');
                }
            }

        }

        $this->mandatoryParams = $mandatoryParams;
        $this->optionalPams = $optionalPams;

    }

    public function hasMandatoryParams(): boolean {
        return !empty($this->mandatoryParams);
    }

    public function getMandatoryParams():Map<string, Variable> {
        return $this->mandatoryParams;
    }

    public function hasOptionalParams(): boolean {
        return !empty($this->optionalPams);
    }

    public function getOptionalParams(): Map<string, Variable> {
        return $this->optionalPams;
    }

}
