<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\Validators\Validator;
use APIs\Domain\Configurations\RawConfiguration;
use APIs\Domain\Configurations\Functions\FunctionCode;
use APIs\Infrastructure\Objects\ConcreteValidator;
use APIs\Domain\Configurations\Validators\ValidatorAdapter;
use APIs\Domain\Configurations\Functions\FunctionCodeAdapter;
use APIs\Domain\Configurations\Views\View;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;

final class ConcreteValidatorAdapter implements ValidatorAdapter {
    private $functionCodeAdapter;
    private $views;
    private $renderedParsers;
    public function __construct(FunctionCodeAdapter $functionCodeAdapter, Map<string, View> $views, Map<string, RenderedParser> $renderedParsers) {
        $this->functionCodeAdapter = $functionCodeAdapter;
        $this->views = $views;
        $this->renderedParsers = $renderedParsers;
    }

    public function convertRawConfigurationToValidators(RawConfiguration $configuration): Map<string, Validator> {

        $validators = $configuration->get('validators');
        $validators = $this->replaceFunctions($validators);
        $validators = $this->replaceSubValidators($validators);
        return $this->createValidators($validators);

    }

    private function createValidator(Map<string, mixed> $validator) {

        if (!isset($validator['function'])) {
            throw new \Exception('There is a validator that does not contain a valid function pointer.');
        }

        $view = null;
        if (isset($validator['view'])) {
            $view = $this->createView($validator['view']);
        }

        $renderedParser = null;
        if (isset($validator['rendered_parser'])) {
            $renderedParser = $this->createRenderedParser($validator['rendered_parser']);
        }

        $subValidators = null;
        if (isset($validator['sub_validators'])) {
            $subValidators = $this->createValidators($validator['sub_validators']);
        }

        return new ConcreteValidator($validator['function'], $subValidators, $view, $renderedParser);
    }

    private function createView(string $keyname) {

        if (!isset($this->views[$keyname])) {
            throw new \Exception('There is a Validator that contains a view ('.$keyname.') that is not defined in the views section.');
        }

        return $this->views[$keyname];

    }

    private function createRenderedParser(string $keyname) {

        if (!isset($this->renderedParsers[$keyname])) {
            throw new \Exception('There is a Validator that contains a rendered_parser ('.$keyname.') that is not defined in the rendered_parser section.');
        }

        return $this->renderedParsers[$keyname];
    }

    private function createValidators(Map<string, mixed> $validators ) {

        $output = Map<string, Validator> {};
        foreach($validators as $keyname => $oneValidator) {
            $output[$keyname] = $this->createValidator($oneValidator);
        }

        return $output;

    }

    private function replaceSubValidators(Map<string, mixed> $validators) {

        foreach($validators as $keyname => $oneValidator) {

            if (isset($oneValidator['sub_validators'])) {

                $subValidators = Map<string, mixed> {};
                foreach($oneValidator['sub_validators'] as $subValidatorKeyname => $oneValidatorKeyname) {

                    if (!isset($validators[$oneValidatorKeyname])) {
                        throw new \Exception('The input validator ('.$keyname.') contains a sub validator ('.$oneValidatorKeyname.') that does not exists in the input validators section.', 500);
                    }

                    $subValidators[$subValidatorKeyname] = $validators[$oneValidatorKeyname];

                }

                $validators[$keyname]['sub_validators'] = $subValidators;
            }
        }

        return $validators;

    }

    private function replaceFunctions(Map<string, mixed> $validators) {

        $functions = $this->functionCodeAdapter->convertDataToFunctionCodes($validators);
        foreach($validators as $keyname => $oneValidator) {

            if (isset($functions[$keyname])) {
                $validators[$keyname]['function'] = $functions[$keyname];
            }
        }

        return $validators;

    }

}
