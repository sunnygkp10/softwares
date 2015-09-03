<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\Functions\FunctionCodeAdapter;
use APIs\Domain\Configurations\Functions\FunctionCode;

final class ConcreteFunctionCodeAdapter implements FunctionCodeAdapter {

    private $functionCodes;
    public function __construct(Map<string, FunctionCode> $functionCodes) {
        $this->functionCodes = $functionCodes;
    }

    public function convertDataToFunctionCodes(Map<string, mixed> $data): Map<string, FunctionCode> {

        $output = Map<string, FunctionCode> {};
        foreach($data as $keyname => $oneData) {

            try {

                $output[$keyname] = $this->convertDataToFunctionCode($oneData);
            } catch(\Exception $exception) {
                throw new \Exception('The given view was: '.$keyname, 0, $exception);
            }

        }

        return $output;
    }

    public function convertDataToFunctionCode(Map<string, mixed> $data): FunctionCode {

        if (!isset($data['function'])) {
            throw new \Exception('The given element does not have a function.  Functions are mandatory for every sections.');
        }

        if (!isset($this->functionCodes[$data['function']])) {
            throw new \Exception('The given element points to a function ('.$data['function'].') that is not defined in the functions section.');
        }

        return $this->functionCodes[$data['function']];

    }

}
