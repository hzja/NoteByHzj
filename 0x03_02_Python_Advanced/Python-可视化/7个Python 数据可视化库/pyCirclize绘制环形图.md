Python可视化<br />推荐一个非常好用的可视化工具包-**「pyCirclize」**，该软件包是为了在Python中轻松、精美地绘制圆形图形（例如Circos Plot和Chord Diagment）。此外，pyCirclize还实现了生物信息学领域有用的基因组和系统发育树可视化方法。详解介绍如下：
<a name="Gt2WK"></a>
## 安装
pyCirclize包的安装非常简单，按照如下方式即可：
```bash
pip install pycirclize
#或者使用conda安装
conda install -c conda-forge pycirclize
```
<a name="IcZOx"></a>
## 基本案例
pyCirclize包中提供了多个绘图函数用于绘制不同的可视化案例，下面就简单的给大家列举一下：
<a name="KlpVY"></a>
### 基础用法-环形柱形图
```python
from pycirclize import Circos
from pycirclize.utils import ColorCycler
import numpy as np
np.random.seed(0)

sectors = {"A": 10, "B": 20, "C": 15}
circos = Circos(sectors, space=5)
for sector in circos.sectors:
    vmin, vmax = 1, 10
    x = np.linspace(sector.start + 0.5, sector.end - 0.5, int(sector.size))
    y = np.random.randint(vmin, vmax, len(x))
    # Plot bar (default)
    track1 = sector.add_track((80, 100), r_pad_ratio=0.1)
    track1.axis()
    track1.xticks_by_interval(1)
    track1.xticks_by_interval(0.1, tick_length=1, show_label=False)
    track1.bar(x, y)
    # Plot stacked bar with user-specified params
    track2 = sector.add_track((50, 70))
    track2.axis()
    track2.xticks_by_interval(1, outer=False)

    ColorCycler.set_cmap("tab10")
    tab10_colors = [ColorCycler() for _ in range(len(x))]
    track2.bar(x, y, width=1.0, color=tab10_colors, ec="grey", lw=0.5, vmax=vmax * 2)

    ColorCycler.set_cmap("Pastel1")
    pastel_colors = [ColorCycler() for _ in range(len(x))]
    y2 = np.random.randint(vmin, vmax, len(x))
    track2.bar(x, y2, width=1.0, bottom=y, color=pastel_colors, ec="grey", lw=0.5, hatch="//", vmax=vmax * 2)

fig = circos.plotfig()
```
![Example01 of pyCirclize](./img/1697723306245-0191b22a-26ac-40cc-8495-9f113da94367.png "Example01 of pyCirclize")
<a name="HFGKU"></a>
### 环形热力图
```python
from pycirclize import Circos
import numpy as np
np.random.seed(0)

sectors = {"A": 10, "B": 20, "C": 15}
circos = Circos(sectors, space=10)
vmin1, vmax1 = 0, 10
vmin2, vmax2 = -100, 100
for sector in circos.sectors:
    # Plot heatmap 
    track1 = sector.add_track((80, 100))
    track1.axis()
    track1.xticks_by_interval(1)
    data = np.random.randint(vmin1, vmax1 + 1, (4, int(sector.size)))
    track1.heatmap(data, vmin=vmin1, vmax=vmax1, show_value=True)
    # Plot heatmap with labels
    track2 = sector.add_track((50, 70))
    track2.axis()
    x = np.linspace(1, int(track2.size), int(track2.size)) - 0.5
    xlabels = [str(int(v + 1)) for v in x]
    track2.xticks(x, xlabels, outer=False)
    track2.yticks([0.5, 1.5, 2.5, 3.5, 4.5], list("ABCDE"), vmin=0, vmax=5)
    data = np.random.randint(vmin2, vmax2 + 1, (5, int(sector.size)))
    track2.heatmap(data, vmin=vmin2, vmax=vmax2, cmap="viridis", rect_kws=dict(ec="white", lw=1))

circos.colorbar(bounds=(0.35, 0.55, 0.3, 0.01), vmin=vmin1, vmax=vmax1, orientation="horizontal")
circos.colorbar(bounds=(0.35, 0.45, 0.3, 0.01), vmin=vmin2, vmax=vmax2, orientation="horizontal", cmap="viridis")

fig = circos.plotfig()
```
![Example02 of pyCirclize](./img/1697723306180-9b1b9346-5a5e-4b1b-aa17-3a794dba8f45.png "Example02 of pyCirclize")
<a name="DJSH7"></a>
### 和弦图
```python
from pycirclize import Circos
import pandas as pd

# Create matrix data (10 x 10)
row_names = list("ABCDEFGHIJ")
col_names = row_names
matrix_data = [
    [51, 115, 60, 17, 120, 126, 115, 179, 127, 114],
    [108, 138, 165, 170, 85, 221, 75, 107, 203, 79],
    [108, 54, 72, 123, 84, 117, 106, 114, 50, 27],
    [62, 134, 28, 185, 199, 179, 74, 94, 116, 108],
    [211, 114, 49, 55, 202, 97, 10, 52, 99, 111],
    [87, 6, 101, 117, 124, 171, 110, 14, 175, 164],
    [167, 99, 109, 143, 98, 42, 95, 163, 134, 78],
    [88, 83, 136, 71, 122, 20, 38, 264, 225, 115],
    [145, 82, 87, 123, 121, 55, 80, 32, 50, 12],
    [122, 109, 84, 94, 133, 75, 71, 115, 60, 210],
]
matrix_df = pd.DataFrame(matrix_data, index=row_names, columns=col_names)

# Initialize from matrix (Can also directly load tsv matrix file)
circos = Circos.initialize_from_matrix(
    matrix_df,
    space=3,
    r_lim=(93, 100),
    cmap="tab10",
    ticks_interval=500,
    label_kws=dict(r=94, size=12, color="white"),
)

print(matrix_df)
fig = circos.plotfig()
```
![Example03 of pyCirclize](./img/1697723306192-d8aedce6-41d4-4dbe-b06d-ee2e72b38873.png "Example03 of pyCirclize")
<a name="maMWx"></a>
### Circos 图（基因组学）
```python
from pycirclize import Circos
from pycirclize.utils import ColorCycler, load_eukaryote_example_dataset

# Load hg38 dataset (https://github.com/moshi4/pycirclize-data/tree/main/eukaryote/mm10)
chr_bed_file, cytoband_file, chr_links = load_eukaryote_example_dataset("mm10")

# Initialize Circos from BED chromosomes
circos = Circos.initialize_from_bed(chr_bed_file, space=3)
circos.text("Mus musculus\n(mm10)", deg=315, r=150, size=12)

# Add cytoband tracks from cytoband file
circos.add_cytoband_tracks((95, 100), cytoband_file)

# Create chromosome color mapping
ColorCycler.set_cmap("hsv") 
chr_names = [s.name for s in circos.sectors]
colors = ColorCycler.get_color_list(len(chr_names))
chr_name2color = {name: color for name, color in zip(chr_names, colors)}

# Plot chromosome name & xticks
for sector in circos.sectors:
    sector.text(sector.name, r=120, size=10, color=chr_name2color[sector.name])
    sector.get_track("cytoband").xticks_by_interval(
        50000000,
        label_size=8,
        label_orientation="vertical",
        label_formatter=lambda v: f"{v / 1000000:.0f} Mb",
    )

# Plot chromosome link
for link in chr_links:
    region1 = (link.query_chr, link.query_start, link.query_end)
    region2 = (link.ref_chr, link.ref_start, link.ref_end)
    color = chr_name2color[link.query_chr]
    if link.query_chr != link.ref_chr:
        circos.link(region1, region2, color=color)

fig = circos.plotfig()
```
![Example04 of pyCirclize](./img/1697723306105-d03e3ef8-b696-45ca-a8c5-fbb7cd3be778.png "Example04 of pyCirclize")

- 系统发育树(Phylogenetic Tree) 这个功能很强，弥补了Python在这一块绘图的不足··
```python
from pycirclize import Circos
from io import StringIO
from Bio import Phylo

# Load ensembl vertebrates species tree
# https://github.com/Ensembl/ensembl-compara/blob/release/108/conf/vertebrates/species_tree.branch_len.nw
treedata = "(saccharomyces_cerevisiae:0.120618,((caenorhabditis_elegans:0.188645,drosophila_melanogaster:0.154675):0.0001,((ciona_intestinalis:0.151626,ciona_savignyi:0.154164):0.0326426,((eptatretus_burgeri:0.128661,petromyzon_marinus:0.168899):0.0001,(((latimeria_chalumnae:0.119616,(((ornithorhynchus_anatinus:0.126661,(((((phascolarctos_cinereus:0.0374961,vombatus_ursinus:0.0354139):0.0214868,notamacropus_eugenii:0.0630732):0.0121551,sarcophilus_harrisii:0.0663059):0.00210215,monodelphis_domestica:0.0747108):0.041817,(((loxodonta_africana:0.0621637,procavia_capensis:0.0763963):0.0167726,echinops_telfairi:0.0966574):0.0238728,((dasypus_novemcinctus:0.0776614,choloepus_hoffmanni:0.0782886):0.0266673,(((((oryctolagus_cuniculus:0.0775193,ochotona_princeps:0.0912307):0.0254255,(((((chinchilla_lanigera:0.0698084,cavia_porcellus:0.0735416):0.00308896,heterocephalus_glaber_female:0.072806):0.00217288,octodon_degus:0.0761958):0.0218839,(((ictidomys_tridecemlineatus:0.0138395,urocitellus_parryii:0.0127405):0.00604211,marmota_marmota_marmota:0.0187229):0.0396847,sciurus_vulgaris:0.0598896):0.0309107):0.00727734,(dipodomys_ordii:0.100796,(jaculus_jaculus:0.0950593,(nannospalax_galili:0.0884969,((((cricetulus_griseus_chok1gshd:0.0489268,mesocricetus_auratus:0.0523932):0.0162373,peromyscus_maniculatus_bairdii:0.0640027):0.00315764,microtus_ochrogaster:0.0701299):0.0109243,(rattus_norvegicus:0.0621346,(mus_pahari:0.0365789,(mus_caroli:0.0204554,((mus_spretus:0.00899553,mus_spicilegus:0.00851447):0.00109632,(mus_musculus_casteij:0.00514915,(mus_musculus_pwkphj:0.00539701,(mus_musculus_wsbeij:0.00197001,((((mus_musculus_c57bl6nj:0.000800378,mus_musculus:0.0001):0.000869129,mus_musculus_nzohlltj:0.00153587):0.0001,(((((mus_musculus_c3hhej:0.000402511,mus_musculus_cbaj:0.000517489):0.000210904,mus_musculus_dba2j:0.0011691):0.000339507,(mus_musculus_aj:0.000316111,mus_musculus_balbcj:0.000963889):4.01914e-05):0.000353079,mus_musculus_akrj:0.000666959):0.0001,(mus_musculus_fvbnj:0.000908422,mus_musculus_nodshiltj:0.00149158):0.000254351):0.000381522):0.0001,(mus_musculus_lpj:0.0006022,mus_musculus_129s1svimj:0.0004678):0.000975719):0.000395485):0.00281515):0.000394928):0.00551731):0.0108856):0.0168717):0.0231139):0.0169089):0.0121629):0.00963979):0.00750102):0.0001):0.00359398):0.00111401,tupaia_belangeri:0.100279):0.0001,((((prolemur_simus:0.0371668,propithecus_coquereli:0.0381832):0.00519709,microcebus_murinus:0.0413679):0.0273709,otolemur_garnettii:0.0749151):0.00720083,(carlito_syrichta:0.0797557,((((saimiri_boliviensis_boliviensis:0.0262902,cebus_capucinus:0.0260398):0.00296817,callithrix_jacchus:0.0297868):0.0001,aotus_nancymaae:0.0253417):0.0236065,((((((pan_paniscus:0.00333033,pan_troglodytes:0.00220967):0.00431356,homo_sapiens:0.00659644):0.00186391,gorilla_gorilla:0.0085649):0.00847511,pongo_abelii:0.0160698):0.00267671,nomascus_leucogenys:0.0196792):0.0109495,((rhinopithecus_roxellana:0.00209258,rhinopithecus_bieti:0.00301742):0.0143196,(chlorocebus_sabaeus:0.0117095,(((macaca_mulatta:0.00254862,macaca_fascicularis:0.00312138):0.000632109,macaca_nemestrina:0.00431289):0.00440353,(papio_anubis:0.00654335,(cercocebus_atys:0.00568373,mandrillus_leucophaeus:0.00676627):0.000576652):0.00118967):0.00396999):0.00477001):0.0136962):0.0176622):0.0277189):4.07016e-05):0.0214276):0.0001,((erinaceus_europaeus:0.100896,sorex_araneus:0.107814):0.0001,((((((ursus_americanus:0.00358833,ursus_maritimus:0.00330167):0.0140233,ailuropoda_melanoleuca:0.0172617):0.0373367,(mustela_putorius_furo:0.0160833,neovison_vison:0.0160267):0.0389355):0.00651625,((canis_lupus_familiaris:0.00107493,canis_lupus_dingo:0.00129507):0.010237,vulpes_vulpes:0.012223):0.0473109):0.00622806,(((panthera_leo:0.0023694,panthera_pardus:0.0027606):0.00118517,panthera_tigris_altaica:0.00462483):0.00753429,felis_catus:0.0116707):0.0532316):0.0164948,((((rhinolophus_ferrumequinum:0.0774819,myotis_lucifugus:0.0815781):0.0001,pteropus_vampyrus:0.0766756):0.00416725,(equus_caballus:0.00631544,equus_asinus_asinus:0.00672456):0.0746163):0.00461396,((vicugna_pacos:0.0200446,camelus_dromedarius:0.0161754):0.0607905,((((((capra_hircus:0.0114539,ovis_aries_rambouillet:0.0109461):0.0168748,(((bos_indicus_hybrid:0.00208873,bos_taurus:0.00230127):0.00251618,(bos_mutus:0.00164309,bos_grunniens:0.00179691):0.00387632):0.0001,bison_bison_bison:0.00489341):0.0226352):0.00304321,moschus_moschiferus:0.0327688):0.00220821,cervus_hanglu_yarkandensis:0.0320047):0.0381676,(((((delphinapterus_leucas:0.00332618,monodon_monoceros:0.00278382):0.00488592,phocoena_sinus:0.00845408):0.00406932,tursiops_truncatus:0.0131854):0.00918139,physeter_catodon:0.0201713):3.59569e-05,balaenoptera_musculus:0.0188968):0.0427691):0.0102577,(catagonus_wagneri:0.0474384,((((((sus_scrofa_meishan:0.00197802,sus_scrofa_jinhua:0.00185198):0.000268806,sus_scrofa_rongchang:0.00200119):0.000297827,sus_scrofa_tibetan:0.00244924):0.0001,sus_scrofa_bamei:0.00219421):8.26503e-05,sus_scrofa_wuzhishan:0.00278787):0.000548203,((sus_scrofa:0.00166698,sus_scrofa_usmarc:0.00197302):0.0001,((sus_scrofa_pietrain:0.00148887,sus_scrofa_largewhite:0.00135113):4.11336e-05,(sus_scrofa_landrace:0.00145828,(sus_scrofa_berkshire:0.00154658,sus_scrofa_hampshire:0.00123342):0.000121719):3.66268e-05):0.000167893):0.000951618):0.0436318):0.0279788):0.0001):0.00900251):0.00213972):0.0167986):0.0001):0.0001):0.00447302):0.0127389):0.00618972):0.00258334,(((((((notechis_scutatus:0.0138535,pseudonaja_textilis:0.0149065):0.00846025,laticauda_laticaudata:0.0263398):0.0048699,naja_naja:0.03202):0.0836509,anolis_carolinensis:0.118737):0.0001,(podarcis_muralis:0.104185,salvator_merianae:0.104005):0.0130264):0.0121285,sphenodon_punctatus:0.128683):0.000664925,((((gopherus_evgoodei:0.0225752,chelonoidis_abingdonii:0.0226148):0.0124392,(terrapene_carolina_triunguis:0.0125212,chrysemys_picta_bellii:0.0160488):0.0179533):0.0413023,pelodiscus_sinensis:0.0780414):0.0404795,(crocodylus_porosus:0.11261,(struthio_camelus_australis:0.086911,((((gallus_gallus:0.0384215,meleagris_gallopavo:0.0409985):0.00582888,coturnix_japonica:0.0442711):0.0314855,(anser_brachyrhynchus:0.03158,anas_platyrhynchos_platyrhynchos:0.02835):0.0401148):0.00622001,((strigops_habroptila:0.0588186,aquila_chrysaetos_chrysaetos:0.0541014):0.0179818,((((serinus_canaria:0.0406713,taeniopygia_guttata:0.0411087):0.0001,geospiza_fortis:0.0371445):0.0117263,parus_major:0.0450985):0.000985951,ficedula_albicollis:0.0453808):0.0263227):0.0135466):0.00101352):0.022625):0.001615):0.0186513):0.00606934):0.0124907,(xenopus_tropicalis:0.13421,leptobrachium_leishanense:0.11976):0.0105643):0.0001):0.01745,(erpetoichthys_calabaricus:0.131615,(lepisosteus_oculatus:0.157564,((paramormyrops_kingsleyae:0.119026,scleropages_formosus:0.114334):0.0157018,(((denticeps_clupeoides:0.120175,clupea_harengus:0.101885):0.00936377,((((astyanax_mexicanus:0.0832734,pygocentrus_nattereri:0.0897366):0.0152395,electrophorus_electricus:0.107845):0.0044232,ictalurus_punctatus:0.100599):0.00947056,(((sinocyclocheilus_grahami:0.0382189,cyprinus_carpio_carpio:0.0330911):0.00869328,carassius_auratus:0.0417317):0.0358928,danio_rerio:0.0782245):0.0323979):0.000399039):0.000838201,((((((oncorhynchus_kisutch:0.014576,oncorhynchus_tshawytscha:0.014744):0.00327522,oncorhynchus_mykiss:0.0161948):0.0125529,(salmo_salar:0.0105392,salmo_trutta:0.0124108):0.0171154):0.00100227,hucho_hucho:0.0337848):0.0551761,esox_lucius:0.100082):0.0320386,(gadus_morhua:0.112842,(myripristis_murdjan:0.0864028,(((((((seriola_lalandi_dorsalis:0.0127856,seriola_dumerili:0.0127744):0.0430594,lates_calcarifer:0.0602556):0.0256458,(cynoglossus_semilaevis:0.0966934,scophthalmus_maximus:0.0988066):0.0001):0.0001,((betta_splendens:0.0848979,anabas_testudineus:0.0721821):0.00871971,mastacembelus_armatus:0.0800653):0.00740968):0.0125545,(((((amphiprion_ocellaris:0.0067724,amphiprion_percula:0.0055076):0.0270587,acanthochromis_polyacanthus:0.0398963):0.0210522,stegastes_partitus:0.0519299):0.0269526,((((((astatotilapia_calliptera:0.00240151,maylandia_zebra:0.00219849):0.000565748,pundamilia_nyererei:0.00882925):0.000672583,haplochromis_burtoni:0.00745383):0.00812133,neolamprologus_brichardi:0.0156631):0.0115662,oreochromis_niloticus:0.0224037):0.0371734,amphilophus_citrinellus:0.055699):0.0214946):0.0178319,(((nothobranchius_furzeri:0.0929233,kryptolebias_marmoratus:0.0884567):0.00806989,((fundulus_heteroclitus:0.080179,cyprinodon_variegatus:0.083961):4.55854e-05,(((poecilia_latipinna:0.00439196,poecilia_formosa:0.00522804):0.0187723,poecilia_reticulata:0.0287077):0.00866388,xiphophorus_maculatus:0.0402887):0.04475):0.0171395):0.00973038,((oryzias_melastigma:0.0447886,oryzias_javanicus:0.0376014):0.0171429,(oryzias_sinensis:0.0204791,oryzias_latipes:0.0182809):0.0449621):0.0495508):0.0001):0.0001):0.0001,(((((larimichthys_crocea:0.0609691,dicentrarchus_labrax:0.0628309):0.0065068,sparus_aurata:0.0681082):0.00615018,((sander_lucioperca:0.0657579,cottoperca_gobio:0.0648421):0.0096021,(gasterosteus_aculeatus:0.0861441,cyclopterus_lumpus:0.0665559):0.0068729):0.000497551):0.00756137,labrus_bergylta:0.0838283):0.00914651,(tetraodon_nigroviridis:0.0695586,takifugu_rubripes:0.0723514):0.0379009):0.000389394):0.0186632,hippocampus_comes:0.114595):0.0001):0.0139491):0.00587418):0.00466696):0.0001):0.00600482):0.00623517):0.0001):9.68556e-05,callorhinchus_milii:0.145499):0.00459101):0.042857):0.00997943):0.120618);"
tree = Phylo.read(StringIO(treedata), "newick")

# Initialize circos sector with tree size
circos = Circos(sectors={"Tree": tree.count_terminals()})
sector = circos.sectors[0]

# Plot tree
track = sector.add_track((30, 100))
track.tree(tree, leaf_label_size=6)

fig = circos.plotfig()
```
![Example05 of pyCirclize](./img/1697723306189-9234c308-d8a8-43f7-9dca-6721b76f471d.png "Example05 of pyCirclize")
