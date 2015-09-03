<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\RESTAPIs\RESTAPIAdapter;
use APIs\Domain\Configurations\Functions\FunctionCodeAdapter;
use APIs\Domain\Configurations\Params\ParamAdapter;
use APIs\Domain\Configurations\Logics\Logic;
use APIs\Domain\Configurations\Views\View;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;
use APIs\Domain\Configurations\RawConfiguration;
use APIs\Infrastructure\Objects\ConcreteRESTAPI;

final class ConcreteRESTAPIAdapter implements RESTAPIAdapter {
    private $functionCodeAdapter;
    private $paramAdapter;
    private $logics;
    private $views;
    private $renderedParsers;
    public function __construct(FunctionCodeAdapter $functionCodeAdapter, ParamAdapter $paramAdapter, Map<string, Logic> $logics, Map<string, View> $views, Map<string, RenderedParser> $renderedParsers) {
        $this->functionCodeAdapter = $functionCodeAdapter;
        $this->paramAdapter = $paramAdapter;
        $this->logics = $logics;
        $this->views = $views;
        $this->renderedParsers = $renderedParsers;
    }

    public function convertRawConfigurationToRESTAPIs(RawConfiguration $configuration): Map<string, RESTAPI> {

        $apis = $configuration->get('rest_apis');
        $apis = $this->replace($apis);

        return $this->createRESTAPIs($apis);

    }

    private function createRESTAPIs(Map<string, mixed> $data) {

        $output = Map<string, RESTAPI> {};
        foreach($data as $keyname => $element) {
            $output[$keyname] = $this->createRESTAPI($element);
        }

        return $output;

    }

    private function createRESTAPI(Map<string, mixed> $data) {

        $subAPIs = null;
        if (isset($data['sub_rest_apis'])) {
            $subAPIs = $this->createRESTAPIs($data['sub_rest_apis']);
        }

        $successAPI = null;
        if (isset($data['if_success_api'])) {
            $successAPI = $this->createRESTAPI($data['if_success_api']);
        }

        $failureAPI = null;
        if (isset($data['if_failure_api'])) {
            $failureAPI = $this->createRESTAPI($data['if_failure_api']);
        }

        $params = isset($data['params']) ? $data['params'] : null;
        $subLogics = isset($data['sub_logics']) ? $data['sub_logics'] : null;
        $subRESTAPIs = isset($data['sub_rest_apis']) ? $subAPIs : null;
        $renderedParser = isset($data['rendered_parser']) ? $data['rendered_parser'] : null;
        $view = isset($data['view']) ? $data['view'] : null;

        return new ConcreteRESTAPI($data['function'], $params, $subRESTAPIs, $subLogics, $view, $renderedParser, $successAPI, $failureAPI);

    }

    private function replace(Map<string, mixed> $apis) {

        $functionCodes = $this->functionCodeAdapter->convertDataToFunctionCodes($apis);
        foreach($apis as $keyname => $oneAPI) {

            $apis[$keyname]['function'] = $functionCodes[$keyname];

            if (isset($oneAPI['params'])) {
                $apis[$keyname]['params'] = $this->paramAdapter->convertDataToParams($oneAPI['params']);
            }

            if (isset($oneAPI['sub_logics'])) {

                $subLogics = Map<string, Logic> {};
                foreach($oneAPI['sub_logics'] as $subLogicKeyname => $logicKeyname) {

                    if (!isset($this->logics[$logicKeyname])) {
                        throw new \Exception('The REST API ('.$keyname.') contains a sub_logic ('.$logicKeyname.') that does not exists in the logics section.');
                    }

                    $subLogics[$subLogicKeyname] = $this->logics[$logicKeyname];

                }

                $apis[$keyname]['sub_logics'] = $subLogics;

            }

            if (isset($oneAPI['view'])) {
                if (!isset($this->views[$oneAPI['view']])) {
                    throw new \Exception('The REST API ('.$keyname.') contains a view ('.$oneAPI['view'].') that does not exists in the views section.');
                }

                $apis[$keyname]['view'] = $this->views[$oneAPI['view']];
            }

            if (isset($oneAPI['rendered_parser'])) {
                if (!isset($this->renderedParsers[$oneAPI['rendered_parser']])) {
                    throw new \Exception('The REST API ('.$keyname.') contains a rendered parser ('.$oneAPI['rendered_parser'].') that does not exists in the rendered_parsers section.');
                }

                $apis[$keyname]['rendered_parser'] = $this->renderedParsers[$oneAPI['rendered_parser']];
            }

            if (isset($oneAPI['sub_rest_apis'])) {

                $subRESTAPIs = Map<string, RESTAPI> {};
                foreach($oneAPI['sub_rest_apis'] as $subRESTKeyname => $restKeyname) {

                    if (!isset($apis[$restKeyname])) {
                        throw new \Exception('The REST API ('.$keyname.') contains a sub_rest_api ('.$restKeyname.') that does not exists in the rest_apis section.');
                    }

                    $subRESTAPIs[$subRESTKeyname] = $apis[$restKeyname];
                }

                $apis[$keyname]['sub_rest_apis'] = $subRESTAPIs;
            }

            if (isset($oneAPI['if_success_api'])) {

                if (!isset($apis[$oneAPI['if_success_api']])) {
                    throw new \Exception('The REST API ('.$keyname.') contains a if_success_api ('.$oneAPI['if_success_api'].') that does not exists in the rest_apis section.');
                }

                $apis[$keyname]['if_success_api'] = $apis[$oneAPI['if_success_api']];
            }

            if (isset($oneAPI['if_failure_api'])) {

                if (!isset($apis[$oneAPI['if_failure_api']])) {
                    throw new \Exception('The REST API ('.$keyname.') contains a if_failure_api ('.$oneAPI['if_failure_api'].') that does not exists in the rest_apis section.');
                }

                $apis[$keyname]['if_failure_api'] = $apis[$oneAPI['if_failure_api']];
            }

        }

        return $apis;
    }

}
