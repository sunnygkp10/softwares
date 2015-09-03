<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\Logics\LogicAdapter;
use APIs\Domain\Configurations\RawConfiguration;
use APIs\Infrastructure\Objects\ConcreteVariable;
use APIs\Domain\Configurations\Params\Param;
use APIs\Infrastructure\Objects\ConcreteParam;
use APIs\Infrastructure\Objects\ConcreteLogic;
use APIs\Domain\Configurations\Params\ParamAdapter;
use APIs\Domain\Configurations\Functions\FunctionCodeAdapter;

final class ConcreteLogicAdapter implements LogicAdapter {

    private $paramAdapter;
    private $functionCodeAdapter;
    public function __construct(ParamAdapter $paramAdapter, FunctionCodeAdapter $functionCodeAdapter) {
        $this->paramAdapter = $paramAdapter;
        $this->functionCodeAdapter = $functionCodeAdapter;
    }

    public function convertRawConfigurationToLogics(RawConfiguration $configuration): Map<string, Logic> {

        $logics = $configuration->get('logic');

        $logics = $this->replaceFunctions($logics);
        $logics = $this->replaceSubLogicsInLogic($logics);

        $keys = array_keys($logics);
        return $this->createLogics($logics);
    }

    private function createLogics(Map<string, string> $logics) {

        $output = Map<string, Logic> {};
        foreach($logics as $keyname => $oneLogic) {
            $output[$keyname] = $this->createLogic($oneLogic);
        }

        return $output;

    }

    private function createLogic(Map<string, mixed> $logic) {

        $params = null;
        if (isset($logic['params'])) {
            $params = $this->paramAdapter->convertDataToParams($logic['params']);
        }

        $subLogics = null;
        if (isset($logic['sub_logics'])) {
            $subLogics = $this->createLogics($logic['sub_logics']);
        }

        return new ConcreteLogic($logic['function'], $params, $subLogics);

    }

    private function replaceSubLogicsInLogic(Map<string, mixed> $logic) {

        foreach($logic as $keyname => $oneLogic) {

            if (isset($oneLogic['sub_logics'])) {

                $subLogics = Map<string, FunctionCode> {};
                foreach($oneLogic['sub_logics'] as $subLogicKeyname => $logicKeyname) {

                    if (!isset($logic[$logicKeyname])) {
                        throw new \Exception('The logic ('.$keyname.') contains a sub_logic ('.$logicKeyname.') that does not exists in the logic section.');
                    }

                    $subLogics[$subLogicKeyname] = $logic[$logicKeyname];

                }

                $logic[$keyname]['sub_logics'] = $subLogics;

            }

        }

        return $logic;

    }

    private function replaceFunctions(Map<string, mixed> $logics) {

        $functions = $this->functionCodeAdapter->convertDataToFunctionCodes($logics);
        foreach($logics as $keyname => $oneLogic) {

            if (isset($functions[$keyname])) {
                $logics[$keyname]['function'] = $functions[$keyname];
            }
        }

        return $logics;

    }

}
