<?hh
namespace APIs\Infrastructure\Applications;
use APIs\Applications\RESTAPIs\RESTAPIApplication;
use APIs\Domain\Configurations\RESTAPIs\Executables\ExecutableRESTAPI;
use APIs\Domain\Configurations\RESTAPIs\RESTAPI;
use APIs\Domain\Https\Repositories\HttpQueryRepository;
use APIs\Domain\Configurations\Validators\Input\InputValidator;
use APIs\Domain\Https\Repositories\HttpQueryRetrieverCriteria;
use APIs\Domain\Https\Repositories\HttpRetrieverCriteriaAdapter;
use APIs\Domain\Configurations\Validators\Validator;
use APIs\Infrastructure\Objects\RetrieverCriterias\ConcreteHttpQueryRetrieverCriteria;
use APIs\Infrastructure\Objects\ConcreteParam;
use APIs\Domain\Configurations\Logics\Logic;
use APIs\Domain\Configurations\Views\View;
use APIs\Domain\Configurations\Rendered\RenderedValue;
use APIs\Infrastructure\Objects\ConcreteRenderedValue;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;
use APIs\Domain\Configurations\Params\Param;

final class ConcreteRESTAPIApplication implements RESTAPIApplication {
    private $httpQueryRepository;
    private $variableData;
    public function __construct(HttpQueryRepository $httpQueryRepository, Map<string, string> $environmentVariables = null, Map<string, string> $requests = null) {
        $this->httpQueryRepository = $httpQueryRepository;
        $this->variableData = Map<string, mixed> {
            'request' => (empty($requests) ? Map<string, string> {} : $requests),
            'environment_variables' => (empty($environmentVariables) ? Map<string, string> {} : $environmentVariables)
        };

    }

    public function execute(ExecutableRESTAPI $restAPI) {

        if ($restAPI->hasParams()) {
            $params = $restAPI->getParams();
            foreach($params as $keyname => $oneParam) {

                if (!isset($this->variableData['request']['query'])) {
                    $this->variableData['request']['query'] = Map<string, string> {};
                }

                $this->variableData['request']['query'][$keyname] = $oneParam;
            }
        }

        $api = $restAPI->getRESTAPI();
        $this->replaceInputValidators($api);
        $this->replaceVariablesInRESTAPI($api);
        return $this->executeRESTAPI($api);
    }

    private function executeRESTAPI(RESTAPI $restAPI) {

        $restAPIs = Map<string, RESTAPI> {
            'first' => $restAPI
        };

        $output = $this->executeRESTAPIs($restAPIs);
        return $output['first'];

    }

    private function executeRESTAPIs(Map<string, RESTAPI> $restAPIs): Map<string, string> {

        $retrieverCriterias = Map<string, HttpQueryRetrieverCriteria>{};
        foreach($restAPIs as $keyname => $oneRESTAPI) {
            $retrieverCriterias[$keyname] = $this->createRetrieverCritriaFromRESTAPI($oneRESTAPI);
        }

        $output = Map<string, string> {};
        $httpQueries = $this->httpQueryRepository->retrieve($retrieverCriterias);
        foreach($httpQueries as $keyname => $oneHttpQuery) {

            $content = $oneHttpQuery->getContent();
            if ($oneHttpQuery->getHttpCode() != 200) {
                throw new \Exception($content, $oneHttpQuery->getHttpCode());
            }

            $output[$keyname] = $content;

        }

        return $output;

    }

    private function createRetrieverCritriaFromRESTAPI(RESTAPI $restAPI) {

        $data = $this->createRESTAPIData($restAPI);

        //retrieve the function code:
        $language = $restAPI->getFunctionCode()->getLanguage();

        $host = $language->getHost();
        $port = $language->getPort();

        return new ConcreteHttpQueryRetrieverCriteria($host, 'post', $port, $data);

    }

    private function getParamData(Param $param) {

        if ($param->isValue()) {
            return $param->getValue();
        }

        $renderedValue = $param->getRenderedValue();
        return $this->getRenderedValueData($renderedValue);
    }

    private function getRenderedValueData(RenderedValue $renderedValue) {

        $functionCode = $renderedValue->getRenderedParser()->getFunctionCode();
        return Map<string, mixed> {
            'input' => $renderedValue->getValue(),
            'name' => $functionCode->getName(),
            'code' => $functionCode->getCode(),
        };

    }

    private function createRESTAPIData(RESTAPI $restAPI) {

        $functionCode = $restAPI->getFunctionCode();
        $data = Map<string, mixed> {
            'name' => $functionCode->getName(),
            'code' => $functionCode->getCode(),
            'type' => 'rest_api'
        };

        if ($restAPI->hasParams()) {
            $params = $restAPI->getParams();
            $data['params'] = Map<string, mixed> {};
            foreach($params as $keyname => $oneParam) {
                $data['params'][$keyname] = $this->getParamData($oneParam);
            }

        }

        if ($restAPI->hasSubLogics()) {
            $subLogics = $restAPI->getSubLogics();
            $data['sub_functions'] = $this->createLogicsData($subLogics);
        }

        if ($restAPI->hasSubRESTAPIs()) {
            $subAPIs = $restAPI->getSubRESTAPIs();
            $data['sub_rest_apis'] = $this->createSubRESTAPIs($subAPIs);
        }

        if ($restAPI->hasView()) {
            $view = $restAPI->getView();
            $data['view'] = $this->createViewData($view);
        }

        return $data;

    }

    private function createSubRESTAPIs(Map<string, RESTAPI> $restAPIs) {

        $output = Map<string, mixed> {};
        $executedRESTAPIs = $this->executeRESTAPIs($restAPIs);
        foreach($executedRESTAPIs as $keyname => $value) {

            if (!isset($restAPIs[$keyname])) {
                throw new \Exception('The executed subAPIs contain this keyname ('.$keyname.'), therefore the restAPIs map should contain it as well.');
            }

            if (!$restAPIs[$keyname]->hasRenderedParser()) {
                throw new \Exception('The subAPI with keyname ('.$keyname.') must have a rendered_parser in order to parse its executed output back into its RESTAPI parent object.');
            }

            $renderedParser = $restAPIs[$keyname]->getRenderedParser();
            $functionCode = $renderedParser->getFunctionCode();

            $output[$keyname] = Map<string, mixed> {
                'name' => $functionCode->getName(),
                'code' => $functionCode->getCode(),
                'value' => $value

            };
        }

        return $output;

    }

    private function createViewsData(Map<string, View> $views) {

        $output = Map<string, Map<string, mixed>> {};
        foreach($views as $keyname => $oneView) {
            $output[$keyname] = $this->createViewData($oneView);
        }

        return $output;

    }

    private function createViewData(View $view) {

        $functionCode = $view->getFunctionCode();
        $data = Map<string, mixed> {
            'name' => $functionCode->getName(),
            'code' => $functionCode->getCode()
        };

        if ($view->hasSubViews()) {
            $subViews = $view->getSubViews();
            $data['sub_views'] = $this->createViewsData($subViews);
        }

        if ($view->hasView()) {
            $oneView = $view->getView();
            $data['view'] = $this->createViewData($oneView);
        }

        return $data;

    }

    private function createLogicsData(Map<string, Logic> $logics) {

        $data = Map<string, Map<string, mixed>> {};
        foreach($logics as $keyname => $oneLogic) {
            $data[$keyname] = $this->createLogicData($oneLogic);
        }

        return $data;

    }

    private function createLogicData(Logic $logic): Map<string, mixed> {

        $functionCode = $logic->getFunctionCode();
        $data = Map<string, mixed> {
            'name' => $functionCode->getName(),
            'code' => $functionCode->getCode()

        };

        if ($logic->hasParams()) {
            $params = $logic->getParams();
            $data['params'] = Map<string, mixed> {};
            foreach($params as $keyname => $oneParam) {
                $data['params'][$keyname] = $oneParam->getValue();
            }

        }

        if ($logic->hasSubLogics()) {
            $subLogics = $logic->getSubLogics();
            $data['sub_logics'] = $this->createLogicsData($subLogics);
        }

        return $data;

    }

    private function replaceVariablesInRESTAPI(RESTAPI &$restAPI) {

        if ($restAPI->hasSubLogics()) {
            $subLogics = $restAPI->getSubLogics();
            $this->replaceVariablesInLogics($subLogics);
        }

        if ($restAPI->hasSubRESTAPIs()) {
            $subRESTAPIs = $restAPI->getSubRESTAPIs();
            $this->replaceVariablesInRESTAPIs($subRESTAPIs);
        }

        if ($restAPI->hasSuccessAPI()) {
            $successAPI = $restAPI->getSuccessAPI();
            $this->replaceVariablesInRESTAPI($successAPI);
        }

        if ($restAPI->hasFailureAPI()) {
            $failureAPI = $restAPI->getFailureAPI();
            $this->replaceVariablesInRESTAPI($failureAPI);
        }

        if (!$restAPI->hasParams()) {
            return;
        }

        $params = $restAPI->getParams();
        foreach($params as $keyname => $oneParam) {

            if (!$oneParam->isVariable()) {
                continue;
            }

            $value = $oneParam->getVariable()->fetch($this->variableData);
            $valueParam = new ConcreteParam($value);
            $restAPI->replaceParam($keyname, $valueParam);
        }

    }

    private function replaceVariablesInRESTAPIs(Map<string, RESTAPI> &$restAPIs) {

        foreach($restAPIs as $oneRESTAPI) {
            $this->replaceVariablesInRESTAPI($oneRESTAPI);
        }

    }

    private function replaceVariablesInLogics(Map<string, Logic> &$logics) {

        foreach($logics as $oneLogic) {
            $this->replaceVariablesInLogic($oneLogic);
        }

    }

    private function replaceVariablesInLogic(Logic &$logic) {

        if ($logic->hasSubLogics()) {
            $subLogics = $logic->getSubLogics();
            $this->replaceVariablesInLogics($subLogics);
        }

        if (!$logic->hasParams()) {
            return;
        }

        $params = $logic->getParams();
        foreach($params as $keyname => $oneParam) {

            if (!$oneParam->isVariable()) {
                continue;
            }

            $value = $oneParam->getVariable()->fetch($this->variableData);
            $valueParam = new ConcreteParam($value);
            $logic->replaceParam($keyname, $valueParam);

        }

    }

    private function replaceInputValidators(RESTAPI &$restAPI) {

        $createParam = function(string $content, RenderedParser $renderedParser = null) {

            if (empty($renderedParser)) {
                return new ConcreteParam($content);
            }

            $renderedValue = new ConcreteRenderedValue($content, $renderedParser);
            return new ConcreteParam(null, null, null, $renderedValue);
        };

        $paramsToExecute = Map<string, mixed> {};
        $this->flatternInputValidatorParamsInRESTAPI($restAPI, $paramsToExecute);
        $results = $this->executeInputValidators($paramsToExecute);

        //replace the content:
        foreach($paramsToExecute as $paramHash => $oneParam) {

            $httpCode = $results['http_queries'][$paramHash]->getHttpCode();
            $content = $results['http_queries'][$paramHash]->getContent();
            $renderedParser = isset($results['rendered_parsers'][$paramHash]) ? $results['rendered_parsers'][$paramHash] : null;
            $newParam = $createParam($content, $renderedParser);

            if ($httpCode != 200) {

                print_r(array($results['http_queries'][$paramHash]));
                die();

                throw new \Exception('There was a problem when executing an input validator.  It returned this http code: '.$httpCode.'.  Its content was: '.$content);
            }

            if (isset($oneParam['rest_apis'])) {

                foreach($oneParam['rest_apis'] as $oneRESTAPI) {
                    $restAPIParams = $oneRESTAPI->getParams();
                    foreach($restAPIParams as $keyname => $oneRESTAPIParam) {
                        $hash = md5(serialize($oneRESTAPIParam));
                        if ($hash == $paramHash) {
                            $oneRESTAPI->replaceParam($keyname, $newParam);
                        }
                    }
                }

            }

            if (isset($oneParam['logics'])) {

                foreach($oneParam['logics'] as $oneLogic) {
                    $logicParams = $oneLogic->getParams();
                    foreach($logicParams as $keyname => $oneLogicParam) {
                        $hash = md5(serialize($oneLogicParam));
                        if ($hash == $paramHash) {
                            $oneLogic->replaceParam($keyname, $newParam);
                        }
                    }

                }
            }

        }

        return;

    }

    private function executeInputValidators(Map<string, mixed> $paramsToExecute) {

        $recursivelyCreateFunctionData = function(Validator $validator) use(&$recursivelyCreateFunctionData) {

            $functionCode = $validator->getFunctionCode();
            $passedParams = Map<string, mixed> {
                'name' => $functionCode->getName(),
                'code' => $functionCode->getCode()
            };

            if ($validator->hasView()) {
                $view = $validator->getView();
                $passedParams['view'] = $this->createViewData($view);
            }

            if (!$validator->hasSubValidators()) {
                return $passedParams;
            }

            $subValidatorParams = Map<string, mixed> {};
            $subValidators = $validator->getSubValidators();
            foreach($subValidators as $keyname => $oneSubValidator) {
                $subValidatorParams[$keyname] = $recursivelyCreateFunctionData($oneSubValidator);
            }

            $passedParams['sub_functions'] = $subValidatorParams;
            return $passedParams;

        };

        $retrieverCriterias = Map<string, HttpQueryRetrieverCriteria> {};
        $renderedParsers = Map<string, RenderedParser> {};
        foreach($paramsToExecute as $keyname => $oneParam) {

            //retrieve the variable keynames:
            $input = $oneParam['input_validator']->getVariable()->fetch($this->variableData);

            //retrieve the function code:
            $validator = $oneParam['input_validator']->getValidator();
            $language = $validator->getFunctionCode()->getLanguage();

            //create the passed params:
            $passedData = $recursivelyCreateFunctionData($validator, $input);
            $passedData['input'] = $input;
            $passedData['type'] = 'input_validator';

            $host = $language->getHost();
            $port = $language->getPort();

            $retrieverCriterias[$keyname] = new ConcreteHttpQueryRetrieverCriteria($host, 'post', $port, $passedData);

            if ($validator->hasRenderedParser()) {
                $renderedParsers[$keyname] = $validator->getRenderedParser();
            }
        }

        $httpQueries = $this->httpQueryRepository->retrieve($retrieverCriterias);
        return Map<string, mixed> {
            'http_queries' => $httpQueries,
            'rendered_parsers' => $renderedParsers
        };

    }

    private function flatternInputValidatorParamsInRESTAPI(RESTAPI &$restAPI, Map<string, mixed> &$output) {

        if ($restAPI->hasSubLogics()) {
            $subLogics = $restAPI->getSubLogics();
            $this->flatternInputValidatorParamsInLogics($subLogics, $output);
        }

        if ($restAPI->hasSubRESTAPIs()) {
            $subRESTAPIs = $restAPI->getSubRESTAPIs();
            $this->flatternInputValidatorParamsInRESTAPIs($subRESTAPIs, $output);
        }

        if ($restAPI->hasSuccessAPI()) {
            $successAPI = $restAPI->getSuccessAPI();
            $this->flatternInputValidatorParamsInRESTAPI($successAPI, $output);
        }

        if ($restAPI->hasFailureAPI()) {
            $failureAPI = $restAPI->getFailureAPI();
            $this->flatternInputValidatorParamsInRESTAPI($failureAPI, $output);
        }

        if (!$restAPI->hasParams()) {
            return;
        }

        $params = $restAPI->getParams();
        foreach($params as $paramName => $oneParam) {

            if (!$oneParam->isInputValidator()) {
                continue;
            }

            $inputValidator = $oneParam->getInputValidator();
            $paramHash = md5(serialize($oneParam));

            if (isset($output[$paramHash])) {

                if(!isset($output[$paramHash]['rest_apis'])) {
                    $output[$paramHash]['rest_apis'] = Vector<RESTAPI> {};
                }

                $output[$paramHash]['rest_apis'][] = $restAPI;
                continue;
            }

            $output[$paramHash] = Map<string, mixed> {
                'rest_apis' => Vector<RESTAPI> {$restAPI},
                'input_validator' => $inputValidator
            };

        }

    }

    private function flatternInputValidatorParamsInRESTAPIs(Map<string, RESTAPI> &$restAPIs, Map<string, mixed> &$output) {

        foreach($restAPIs as $oneRESTAPI) {
            $this->flatternInputValidatorParamsInRESTAPI($oneRESTAPI, $output);
        }

    }

    private function flatternInputValidatorParamsInLogics(Map<string, Logic> &$logics, Map<string, mixed> &$output) {

        foreach($logics as $oneLogic) {
            $this->flatternInputValidatorParamsInLogic($oneLogic, $output);
        }

    }

    private function flatternInputValidatorParamsInLogic(Logic &$logic, Map<string, mixed> &$output) {

        if ($logic->hasSubLogics()) {
            $subLogics = $logic->getSubLogics();
            $this->flatternInputValidatorParamsInLogics($subLogics, $output);
        }

        if (!$logic->hasParams()) {
            return;
        }

        $params = $logic->getParams();
        foreach($params as $paramName => $oneParam) {

            if (!$oneParam->isInputValidator()) {
                continue;
            }

            $inputValidator = $oneParam->getInputValidator();
            $paramHash = md5(serialize($oneParam));

            if (isset($output[$paramHash])) {

                if(!isset($output[$paramHash]['logics'])) {
                    $output[$paramHash]['logics'] = Vector<Logic> {};
                }

                $output[$paramHash]['logics'][] = $logic;
                return;
            }

            $output[$paramHash] = Map<string, mixed> {
                'logics' => Vector<Logic> {$logic},
                'input_validator' => $inputValidator
            };

        }

    }

}
