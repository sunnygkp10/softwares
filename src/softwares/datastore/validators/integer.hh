<?hh
function integer(string $input, Map<string) {
    $matches = array();
    preg_match_all('/[0-9]+/s', $input, $matches);

    if (isset($matches[0][0]) && ($matches[0][0] == $input)) {
        return (int) $matches[0][0];
    }

    throw new \Exception('The given input ('.$input.') is not a valid integer.', 500);
};
