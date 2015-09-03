<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteHttpRoute;

final class ConcreteHttpRouteTest extends \PHPUnit_Framework_TestCase {
    private $httpRoutePatternMock;
    private $httpMethod;
    private $uppercaseHttpMethod;
    private $invalidHttpMethod;
    private $emptyHttpMethod;
    public function setUp() {

        $this->httpRoutePatternMock = $this->getMock('APIs\Domain\Configurations\Routes\Https\Patterns\HttpRoutePattern');

        $this->httpMethod = 'get';
        $this->uppercaseHttpMethod = 'POST';
        $this->invalidHttpMethod = 'not an http method.';
        $this->emptyHttpMethod = '';

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $httpRoute = new ConcreteHttpRoute($this->httpRoutePatternMock, $this->httpMethod);

        $this->assertEquals($this->httpRoutePatternMock, $httpRoute->getPattern());
        $this->assertEquals($this->httpMethod, $httpRoute->getMethod());

        $this->assertTrue($httpRoute instanceof \APIs\Domain\Configurations\Routes\Https\HttpRoute);

    }

    public function testCreate_withUppercaseHttpMethod_Success() {

        $httpRoute = new ConcreteHttpRoute($this->httpRoutePatternMock, $this->uppercaseHttpMethod);

        $this->assertEquals($this->httpRoutePatternMock, $httpRoute->getPattern());
        $this->assertEquals(strtolower($this->uppercaseHttpMethod), $httpRoute->getMethod());

    }

    public function testCreate_withInvalidHttpMethod_Success() {

        $asserted = false;
        try {

            new ConcreteHttpRoute($this->httpRoutePatternMock, $this->invalidHttpMethod);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

    public function testCreate_withEmptyHttpMethod_Success() {

        $asserted = false;
        try {

            new ConcreteHttpRoute($this->httpRoutePatternMock, $this->emptyHttpMethod);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

}
