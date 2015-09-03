<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Routes\Https\Patterns\HttpRoutePattern;

final class ConcreteHttpRoutePattern implements HttpRoutePattern {
    private $pattern;
    private $regexPattern;
    private $variables;
    private $variablesWithDelimiters;
    public function __construct(string $pattern, string $prefix, string $suffix) {

        if (strlen($prefix) != 1) {
            throw new \Exception('The prefix '.$prefix.' can only be of 1 character.');
        }

        if (strlen($suffix) != 1) {
            throw new \Exception('The suffix '.$suffix.' can only be of 1 character.');
        }

        $variables = $this->discoverVariables($pattern, $prefix, $suffix);

        $this->variables = !empty($variables['variables'])? $variables['variables'] : null;
        $this->variablesWithDelimiters = !empty($variables['variables_with_delimiters'])? $variables['variables_with_delimiters'] : null;
        $this->pattern = $pattern;
        $this->regexPattern = $this->createRegexPattern($pattern);

    }

    private function discoverVariables(string $pattern, string $prefix, string $suffix): Map<string, Vector<string>> {

        $convert = function(array $variables): Vector<string> {

            $output = Vector<string> {};
            foreach($variables as $oneVariable) {
                $output[] = $oneVariable;
            }

            return $output;

        };

        $matches = array();
        preg_match_all('/\\'.$prefix.'([^'.$suffix.']+)\\'.$suffix.'/s', $pattern, $matches);

        $variablesWithDelimiters = null;
        if (isset($matches[0]) && is_array($matches[0]) && !empty($matches[0])) {
            $variablesWithDelimiters = $convert($matches[0]);
        }

        $variables = null;
        if (isset($matches[1]) && is_array($matches[1]) && !empty($matches[1])) {
            $variables = $convert($matches[1]);
        }

        return Map<string, Vector<string>> {
            'variables' => $variables,
            'variables_with_delimiters' => $variablesWithDelimiters
        };

    }

    private function createRegexPattern(string $pattern) {

        if (empty($this->variablesWithDelimiters)) {
            return null;
        }

        $regex = '';
        $pattern = str_replace('/', '\/', $pattern);
        foreach($this->variablesWithDelimiters as $oneVariable) {
            $pattern = str_replace($oneVariable, '([^\/]+)', $pattern);
        }

        return '/'.$pattern.'/s';
    }

    public function validate($uri): Map<string, string> {

        $convertMatchesToVariables = function(array $matches) {

            $output = Vector<string> {};
            unset($matches[0]);
            foreach($matches as $oneMatch) {

                if (isset($oneMatch[0])) {
                    $output[] = $oneMatch[0];
                }
            }

            return $output;

        };

        if (empty($this->regexPattern)) {
            if ($uri == $this->pattern) {
                return Map<string, string> {};
            }

            throw new \Exception('There is no variable in our pattern('.$this->pattern.').  However, the uri ('.$uri.') does not perfectly match the pattern ('.$this->pattern.').');
        }

        $matches = array();
        preg_match_all($this->regexPattern, $uri, $matches);

        if (!isset($matches[0][0]) || ($matches[0][0] != $uri)) {
            throw new \Exception('The uri ('.$uri.') does not match the regex pattern ('.$this->regexPattern.').');
        }

        $inputVariables = $convertMatchesToVariables($matches);
        $amountVariables = count($this->variables);
        $amountMatches = count($inputVariables);
        if ($amountVariables != $amountMatches) {
            throw new \Exception('The uri ('.$uri.') was supposed to have '.$amountVariables.' variables.  '.$amountMatches.' were found.');
        }

        $output = Map<string, string> {};
        foreach($this->variables as $keyname => $value) {
            $output[$value] = $inputVariables[$keyname];
        }

        return $output;

    }

    public function hasVariables(): boolean {
        return !empty($this->variables);
    }

    public function getPattern(): string {
        return $this->pattern;
    }

    public function getVariables(): ?Vector<string> {
        return $this->variables;
    }

    public function getVariablesWithDelimiters(): ?Vector<string> {
        return $this->variablesWithDelimiters;
    }

}
