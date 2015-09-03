<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\Params\Param;
use APIs\Domain\Configurations\Params\ParamAdapter;
use APIs\Domain\Configurations\Variables\VariableAdapter;
use APIs\Domain\Configurations\Validators\Input\InputValidatorAdapter;
use APIs\Infrastructure\Objects\ConcreteParam;

final class ConcreteParamAdapter implements ParamAdapter {
    private $variableAdapter;
    private $inputValidatorAdapter;
    public function __construct(VariableAdapter $variableAdapter, InputValidatorAdapter $inputValidatorAdapter) {
        $this->variableAdapter = $variableAdapter;
        $this->inputValidatorAdapter = $inputValidatorAdapter;
    }

    public function convertDataToParams(Map<string, mixed> $data): Map<string, ?Param> {
        $output = Map<string, Param> {};
        foreach($data as $keyname => $value) {

            try {

                if (is_null($value)) {
                    $output[$keyname] = null;
                    continue;
                }

                if (is_string($value)) {
                    $output[$keyname] = $this->convertStringToParam($value);
                    continue;
                }
                
                $output[$keyname] = $this->convertDataToParam($value);


            } catch (\Exception $exception) {
                throw new \Exception('The param keyname is: '.$keyname, 0, $exception);
            }

        }

        return $output;

    }

    public function convertDataToParam(Map<string, mixed> $data): Param {
        $inputValidator = $this->inputValidatorAdapter->convertDataToInputValidator($data);
        return new ConcreteParam(null, null, $inputValidator);
    }

    public function convertStringToParam(string $param): Param {

        try {

            $variable = $this->variableAdapter->convertStringToVariable($param);
            return new ConcreteParam(null, $variable);

        } catch (\Exception $exception) {

        }

        return new ConcreteParam($param);

    }

}
