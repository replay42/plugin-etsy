<?hh // strict
namespace Etsy;

use Plenty\Modules\Cron\Services\CronContainer;
use Plenty\Plugin\ServiceProvider;

use Etsy\Contracts\ItemDataProviderContract;
use Etsy\Crons\ItemExportCron;
use Etsy\Crons\ItemUpdateCron;
use Etsy\Crons\OrderImportCron;
use Etsy\Factories\ItemDataProviderFactory;
use Etsy\Providers\ItemExportDataProvider;
use Etsy\Providers\ItemUpdateDataProvider;

class EtsyServiceProvider extends ServiceProvider
{
	public function register():void
	{
		$this->getApplication()->bind('Etsy\item.dataprovider.export', ItemExportDataProvider::class);
		$this->getApplication()->bind('Etsy\item.dataprovider.update', ItemUpdateDataProvider::class);

		$this->getApplication()->singleton(ItemDataProviderFactory::class);

         $this->getApplication()->register(EtsyRouteServiceProvider::class);
	}

    public function boot(CronContainer $container):void
    {
        $container->add(CronContainer::DAILY, ItemExportCron::class);
        $container->add(CronContainer::DAILY, ItemUpdateCron::class);
        $container->add(CronContainer::HOURLY, OrderImportCron::class);
    }
}
