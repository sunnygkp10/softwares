<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteRoute;

final class ConcreteRouteTest extends \PHPUnit_Framework_TestCase {
    private $restAPIMock;
    private $httpRouteMock;
    public function setUp() {

        $this->restAPIMock = $this->getMock('APIs\Domain\Configurations\RESTAPIs\RESTAPI');
        $this->httpRouteMock = $this->getMock('APIs\Domain\Configurations\Routes\Https\HttpRoute');

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $route = new ConcreteRoute($this->restAPIMock, $this->httpRouteMock);

        $this->assertEquals($this->restAPIMock, $route->getRESTAPI());
        $this->assertTrue($route->isHttpRoute());
        $this->assertEquals($this->httpRouteMock, $route->getHttpRoute());

        $this->assertTrue($route instanceof \APIs\Domain\Configurations\Routes\Route);

    }

    public function testCreate_withoutRoute_throwsException() {

        $asserted = false;
        try {

            new ConcreteRoute($this->restAPIMock);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

}
