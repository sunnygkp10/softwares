<?hh
//Map<string, \Closure>
function elementsWithPagination(Map<string, string> $params, Map<string, \Closure> $subViews = null) {

    if (!isset($subViews['single_element'])) {
        throw new \Exception('The single_element subView is mandatory in order to render elements with pagination.');
    }

    if (!isset($params['elements'])) {
        throw new \Exception('The elements param is mandatory in order to render elements with pagination.');
    }

    foreach($params['elements'] as $index => $oneElement) {
        $params['elements'][$index] = $subViews['single_element']($oneElement);
    }

    return $params;

};
