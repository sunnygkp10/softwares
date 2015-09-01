<?hh
function render(Map<string, string> $params, Map<string, \Closure> $subViews = null) {
    return json_encode($params->toArray());
};
