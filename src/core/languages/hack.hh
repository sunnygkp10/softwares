<?hh
function handleExceptions(\Exception $exception) {

    $exceptionToData = function(\Exception $exception) use(&$exceptionToData) {

        $data = array(
            'code' => $exception->getCode(),
            'message' => $exception->getMessage(),
            'file' => $exception->getFile(),
            'line' => $exception->getLine(),
            'trace' => $exception->getTraceAsString()
        );

        $previous = $exception->getPrevious();
        if (!empty($previous)) {
            $data['previous'] = $exceptionToData($previous);
        }

        return $data;

    };

    $data = $exceptionToData($exception);

    http_response_code(500);
    echo json_encode($data);
    return true;
}

function handleError($code, $message, $file, $line) {

    $data = array(
        'code' => $code,
        'message' => $message,
        'file' => $file,
        'line' => $line
    );

    http_response_code(500);
    echo json_encode($data);
    return true;
}

function shutDown() {

}

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);
set_error_handler("handleError");
register_shutdown_function("shutDown");
set_exception_handler("handleExceptions");

$cachePath = '../cache';

$generateRandomFunctionName = function($length = 25) {
    $characters = 'abcdefghijklmnopqrstuvwxyz';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
};

$includeCode = function(string $name, string $code) use($cachePath, $generateRandomFunctionName) {

    $hash = md5($code);
    $fileName = $cachePath.'/'.$hash.'.hh';

    file_put_contents($fileName, $code);
    include($fileName);
    unlink($fileName);

    return $name;

};

$createInputValidator = function(string $name, string $code, mixed $input, Map<string, \Closure> $subFunctions = null) use($includeCode) {

    $functionName = $includeCode($name, $code);
    return function() use($functionName, $input, $subFunctions) {

        if (empty($subFunctions)) {
            return $functionName($input);
        }

        return $functionName($input, $subFunctions);
    };

};

$createRenderedParser = function(string $name, string $code, mixed $input) use($includeCode) {

    $functionName = $includeCode($name, $code);
    return function() use($functionName, $input) {
        return $functionName($input);
    };

};


$createSubLogics = function(array $subFunctions) use(&$createSubLogics, $includeCode) {

    $createSubLogic = function(array $subLogic) use(&$createSubLogics, $includeCode) {

        if (!isset($subLogic['name']) || !isset($subLogic['code'])) {
            $keys = array_keys($subLogic);
            throw new \Exception('The name and code are mandatory in order to create a sub logic.  Keys inside given sublogic data: '.implode(',', $keys));
        }

        $params = null;
        if (isset($subLogic['params'])) {
            $params = Map<string, mixed> {};
            foreach($subLogic['params'] as $keyname => $oneParam) {
                $params[$keyname] = $oneParam;
            }
        }

        $subLogics = null;
        if (isset($subLogic['sub_logics'])) {
            $subLogics = $createSubLogics($subLogic['sub_logics']);
        }

        $functionName = $includeCode($subLogic['name'], $subLogic['code']);
        $fn = function(Map<string, mixed> $additionalParams = null) use($functionName, $params, $subLogics) {

            $mergeParams = function() use($additionalParams, $params) {

                if (empty($additionalParams) && empty($params)) {
                    return null;
                }

                if (!empty($additionalParams) && empty($params)) {
                    return $additionalParams;
                }

                if (!empty($params) && empty($additionalParams)) {
                    return $params;
                }

                foreach($additionalParams as $keyname => $oneParam) {
                    $params[$keyname] = $oneParam;
                }

                return $params;
            };

            $params = $mergeParams();
            if (empty($params) && empty($subLogics)) {
                return $functionName();
            }

            if (empty($subLogics)) {
                return $functionName($params);
            }

            return $functionName($params, $subLogics);

        };

        return $fn;

    };

    $subLogics = Map<string, \Closure> {};
    foreach($subFunctions as $keyname => $oneSubFunction) {
        $subLogics[$keyname] = $createSubLogic($oneSubFunction);
    }

    return $subLogics;

};

$createView = function(array $view) use(&$createView, $includeCode) {

    if (!isset($view['name']) || !isset($view['code'])) {
        throw new \Exception('The name and code params are mandatory in order to create a view closure.');
    }

    $subViews = null;
    if (isset($view['sub_views'])) {
        $subViews = Map<string, mixed> {};
        foreach($view['sub_views'] as $keyname => $oneView) {
            $subViews[$keyname] = $createView($oneView);
        }
    }

    $functionName = $includeCode($view['name'], $view['code']);
    return function(mixed $input) use($functionName, $subViews) {

        if (empty($subViews)) {
            return $functionName($input);
        }

        return $functionName($input, $subViews);

    };

};

$processView = function(mixed $input, array $view) use($createView, &$processView) {

    $fn = $createView($view);
    $output = $fn($input);

    if (isset($view['view'])) {
        $output = $processView($output, $view['view']);
    }

    return $output;

};

$processParam = function(Map<string, mixed> $params) use($createRenderedParser, &$processParam) {

    $convertToMap = function(array $data) use(&$convertToMap) {

        $output = Map<string, mixed> {};
        foreach($data as $keyname => $oneElement) {

            if (is_array($oneElement)) {
                $oneElement = $convertToMap($oneElement);
            }

            $output[$keyname] = $oneElement;
        }

        return $output;

    };

    $output = Map<string, mixed> {};
    foreach($params as $keyname => $oneParam) {

        if (!isset($oneParam['input']) || !isset($oneParam['name']) || !isset($oneParam['code'])) {

            if (is_array($oneParam)) {
                $oneParam = $convertToMap($oneParam);
                $oneParam = $processParam($oneParam);
            }

            $output[$keyname] = $oneParam;
            continue;
        }

        $renderedParser = $createRenderedParser($oneParam['name'], $oneParam['code'], $oneParam['input']);
        $output[$keyname] = $renderedParser();
    }

    return $output;

};

$createRESTAPI = function(string $name, string $code, Map<string, mixed> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subRESTAPIs = null) use($includeCode, $createRenderedParser, $processParam) {

    $params = $processParam($params);
    $functionName = $includeCode($name, $code);
    return function() use($functionName, $params, $subLogics, $subRESTAPIs) {

        if (empty($subRESTAPIs)) {
            return $functionName($params, $subLogics);
        }

        if (empty($subLogics) && empty($subRESTAPIs)) {
            return $functionName($params);
        }

        if (empty($params) && empty($subLogics) && empty($subRESTAPIs)) {
            return $functionName();
        }

        return $functionName($params, $subLogics, $subRESTAPIs);
    };

};

$processInputValidator = function() use($createSubLogics, $createInputValidator, $processView, $includeCode) {

    $convertToMap = function(array $input) use(&$convertToMap) {

        $output = Map<string, mixed> {};
        foreach($input as $keyname => $oneElement) {

            if (is_array($oneElement)) {
                $oneElement = $convertToMap($oneElement);
            }

            $output[$keyname] = $oneElement;
        }

        return $output;

    };

    if (!isset($_POST['name']) || !isset($_POST['code']) || !isset($_POST['input'])) {
        throw new \Exception('The name, code and input params are mandatory in order to process an input validator properly.');
    }

    $subFunctions = null;
    if (isset($_POST['sub_functions'])) {
        $subFunctions = $createSubLogics($_POST['sub_functions']);
    }


    if (is_array($_POST['input'])) {
        $_POST['input'] = $convertToMap($_POST['input']);
    }

    $inputValidator = $createInputValidator($_POST['name'], $_POST['code'], $_POST['input'], $subFunctions);
    $output = $inputValidator();
    if (isset($_POST['view'])) {
        $output = $processView($output, $_POST['view']);
    }

    echo $output;
};

$processRESTAPI = function() use($includeCode, $createSubLogics, $processView, $createRESTAPI) {

    if (!isset($_POST['name']) || !isset($_POST['code'])) {
        throw new \Exception('The name, code and input request params are mandatory in order to process a REST API properly.');
    }

    $params = null;
    if (isset($_POST['params'])) {
        $params = Map<string, mixed> {};
        foreach($_POST['params'] as $keyname => $value) {
            $params[$keyname] = $value;
        }
    }

    $subLogics = null;
    if (isset($_POST['sub_functions'])) {
        $subLogics = $createSubLogics($_POST['sub_functions']);
    }

    $subRESTAPIs = null;
    if (isset($_POST['sub_rest_apis'])) {
        $subRESTAPIs = Map<string, string> {};
        foreach($_POST['sub_rest_apis'] as $keyname => $oneRenderedParser) {

            if (!isset($oneRenderedParser['code']) || !isset($oneRenderedParser['name']) || !isset($oneRenderedParser['value'])) {
                throw new \Exception('The rendered_parser objects, given inside the sub_rest_apis param, must contain a code, name and value.');
            }

            $value = $oneRenderedParser['value'];
            $functionName = $includeCode($oneRenderedParser['name'], $oneRenderedParser['code']);
            $subRESTAPIs[$keyname] = function() use($functionName, $value) {
                return $functionName($value);
            };
        }
    }

    $restAPI = $createRESTAPI($_POST['name'], $_POST['code'], $params, $subLogics, $subRESTAPIs);
    $output = $restAPI();

    if (isset($_POST['view'])) {
        $output = $processView($output, $_POST['view']);
    }

    echo $output;

};

$process = function() use($processInputValidator, $processRESTAPI) {

    $processors = Map<string, \Closure> {
        'input_validator' => $processInputValidator,
        'rest_api' => $processRESTAPI
    };

    if (!isset($_POST['type'])) {
        throw new \Exception('The type is mandatory.  The possible types are: '.implode(',', $processors->keys()));
    }

    if (!isset($processors[$_POST['type']])) {
        throw new \Exception('The given type ('.$_POST['type'].') is invalid.  Only these types are valid: '.implode(',', $processors->keys()));
    }

    $processors[$_POST['type']]();
};

$process();
