<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteView;

final class ConcreteViewTest extends \PHPUnit_Framework_TestCase {
    private $functionCodeMock;
    private $viewMock;
    private $subViews;
    private $subViewsWithView;
    public function setUp() {

        $this->functionCodeMock = $this->getMock('APIs\Domain\Configurations\Functions\FunctionCode');
        $this->viewMock = $this->getMock('APIs\Domain\Configurations\Views\View');
        $this->subViews = Map<string, View> {
            'first' => new ConcreteView($this->functionCodeMock)
        };

        $this->subViewsWithView = Map<string, View> {
            'first' => new ConcreteView($this->functionCodeMock, $this->viewMock)
        };

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $view = new ConcreteView($this->functionCodeMock);

        $this->assertEquals($this->functionCodeMock, $view->getFunctionCode());
        $this->assertFalse($view->hasView());
        $this->assertNull($view->getView());
        $this->assertFalse($view->hasSubViews());
        $this->assertNull($view->getSubViews());

        $this->assertTrue($view instanceof \APIs\Domain\Configurations\Views\View);

    }

    public function testCreate_withView_Success() {

        $view = new ConcreteView($this->functionCodeMock, $this->viewMock);

        $this->assertEquals($this->functionCodeMock, $view->getFunctionCode());
        $this->assertTrue($view->hasView());
        $this->assertEquals($this->viewMock, $view->getView());
        $this->assertFalse($view->hasSubViews());
        $this->assertNull($view->getSubViews());
    }

    public function testCreate_withSubViews_Success() {

        $view = new ConcreteView($this->functionCodeMock, null, $this->subViews);

        $this->assertEquals($this->functionCodeMock, $view->getFunctionCode());
        $this->assertFalse($view->hasView());
        $this->assertNull($view->getView());
        $this->assertTrue($view->hasSubViews());
        $this->assertEquals($this->subViews, $view->getSubViews());
    }

    public function testCreate_withView_withSubViews_Success() {

        $view = new ConcreteView($this->functionCodeMock, $this->viewMock, $this->subViews);

        $this->assertEquals($this->functionCodeMock, $view->getFunctionCode());
        $this->assertTrue($view->hasView());
        $this->assertEquals($this->viewMock, $view->getView());
        $this->assertTrue($view->hasSubViews());
        $this->assertEquals($this->subViews, $view->getSubViews());
    }

    public function testCreate_withSubViewsThatContainsAView_throwsException() {

        $asserted = false;
        try {

            new ConcreteView($this->functionCodeMock, null, $this->subViewsWithView);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);
    }


}
