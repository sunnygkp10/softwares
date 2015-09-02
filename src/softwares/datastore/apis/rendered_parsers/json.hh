<?hh
function parse(string $json) {

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

    $data = json_decode($json, true);
    return $convertToMap($data);
};
