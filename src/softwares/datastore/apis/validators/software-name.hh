<?hh
function softwareName(string $input) {

    $matches = array();
    preg_match_all('/[a-z0-9\_]+/s', $input, $matches);

    if (!isset($matches[0][0]) || ($matches[0][0] != $input)) {
        throw new \Exception('The software name must only contain lower capital letters, numbers and hyphens.', 500);
    }

    return $matches[0][0];
};
