<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteHttpRoutePattern;

final class ConcreteHttpRoutePatternTest extends \PHPUnit_Framework_TestCase {
    private $pattern;
    private $patternWithoutVariables;
    private $prefix;
    private $suffix;
    private $variables;
    private $variablesWithDelimiters;
    public function setUp() {

        $this->pattern = '/*software_name-/*container_name-/*uuid-';
        $this->patternWithoutVariables = '/not-a-variable';
        $this->prefix = '*';
        $this->suffix = '-';

        $this->variables = Vector<string> {
            'software_name', 'container_name', 'uuid'
        };

        $this->variablesWithDelimiters = Vector<string> {
            '*software_name-', '*container_name-', '*uuid-'
        };

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $httpRoutePattern = new ConcreteHttpRoutePattern($this->pattern, $this->prefix, $this->suffix);

        $this->assertEquals($this->pattern, $httpRoutePattern->getPattern());
        $this->assertTrue($httpRoutePattern->hasVariables());
        $this->assertEquals($this->variables, $httpRoutePattern->getVariables());
        $this->assertEquals($this->variablesWithDelimiters, $httpRoutePattern->getVariablesWithDelimiters());

        $this->assertTrue($httpRoutePattern instanceof \APIs\Domain\Configurations\Routes\Https\Patterns\HttpRoutePattern);

    }

    public function testCreate_withoutVariables_Success() {

        $httpRoutePattern = new ConcreteHttpRoutePattern($this->patternWithoutVariables, $this->prefix, $this->suffix);

        $this->assertEquals($this->patternWithoutVariables, $httpRoutePattern->getPattern());
        $this->assertFalse($httpRoutePattern->hasVariables());
        $this->assertNull($httpRoutePattern->getVariables());
        $this->assertNull($httpRoutePattern->getVariablesWithDelimiters());

    }

    public function testCreate_withInvalidPrefix_throwsException() {

        $asserted = false;
        try {

            new ConcreteHttpRoutePattern($this->pattern, 'invalid', $this->suffix);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

    public function testCreate_withInvalidSuffix_throwsException() {

        $asserted = false;
        try {

            new ConcreteHttpRoutePattern($this->pattern, $this->prefix, 'invalid');

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

}
