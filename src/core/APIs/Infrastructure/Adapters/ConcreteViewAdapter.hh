<?hh
namespace APIs\Infrastructure\Adapters;
use APIs\Domain\Configurations\Views\ViewAdapter;
use APIs\Domain\Configurations\RawConfiguration;
use APIs\Infrastructure\Objects\ConcreteView;
use APIs\Domain\Configurations\Functions\FunctionCodeAdapter;

final class ConcreteViewAdapter implements ViewAdapter {

    private $functionCodeAdapter;
    public function __construct(FunctionCodeAdapter $functionCodeAdapter) {
        $this->functionCodeAdapter = $functionCodeAdapter;
    }

    public function convertRawConfigurationToViews(RawConfiguration $configuration): Map<string, View> {

        $views = $configuration->get('views');
        $views = $this->replaceFunctions($views);
        $views = $this->replaceView($views);
        $views = $this->replaceSubViews($views);
        return $this->createViews($views);
    }

    private function createViews(Map<string, mixed> $views) {

        $output = Map<string, View> {};
        foreach($views as $keyname => $oneView) {
            $output[$keyname] = $this->createView($oneView);
        }

        return $output;

    }

    private function createView(Map<string, mixed> $view) {

        $createdView = null;
        if (isset($view['view'])) {
            $createdView = $this->createView($view['view']);
        }

        $subViews = null;
        if (isset($view['sub_views'])) {
            $subViews = $this->createViews($view['sub_views']);
        }

        return new ConcreteView($view['function'], $createdView, $subViews);

    }

    private function replaceSubViews(Map<string, mixed> $views) {

        foreach($views as $keyname => $oneView) {

            if (!isset($oneView['sub_views'])) {
                continue;
            }

            $subViews = Map<string, mixed> {};
            foreach($oneView['sub_views'] as $subViewKeyname => $viewKeyname) {

                if (!isset($views[$viewKeyname])) {
                    throw new \Exception('The view ('.$keyname.') contains a sub_view ('.$viewKeyname.') that does not exists in the views section.');
                }

                $subViews[$subViewKeyname] = $views[$viewKeyname];
            }

            if (!empty($subViews)) {
                $views[$keyname]['sub_views'] = $subViews;
            }

        }

        return $views;

    }

    private function replaceView(Map<string, mixed> $views) {

        foreach($views as $keyname => $oneView) {

            if (!isset($oneView['view'])) {
                continue;
            }

            if (!isset($views[$oneView['view']])) {
                throw new \Exception('The view ('.$keyname.') points to a view ('.$oneView['view'].') that is not defined in the views section.');
            }

            $views[$keyname]['view'] = $views[$oneView['view']];

        }

        return $views;

    }

    private function replaceFunctions(Map<string, mixed> $views) {

        $functions = $this->functionCodeAdapter->convertDataToFunctionCodes($views);
        foreach($views as $keyname => $oneView) {

            if (isset($functions[$keyname])) {
                $views[$keyname]['function'] = $functions[$keyname];
            }
        }

        return $views;

    }

}
