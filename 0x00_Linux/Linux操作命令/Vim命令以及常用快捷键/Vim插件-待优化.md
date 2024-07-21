Linux Vim
<a name="vY3L4"></a>
## Vim配置文件-.vimrc
.vimrc 是 Vim 的配置文件，需要自行创建：
```bash
cd Home               // 进入 Home 目录
touch .vimrc          // 配置文件
# Unix
# vim-plug
# Vim
curl -fLo ~/.vim/autoload/plug.vim --create-dirs \
    https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
# Neovim
curl -fLo ~/.local/share/nvim/site/autoload/plug.vim --create-dirs \
    https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
```
其他平台，可以查看 [vim-plug](https://github.com/junegunn/vim-plug)。<br />[https://github.com/junegunn/vim-plug](https://github.com/junegunn/vim-plug)
<a name="xNv4n"></a>
## 插件配置
Vim插件参考配置：[https://github.com/FengShangWuQi/to-vim/blob/master/.vimrc](https://github.com/FengShangWuQi/to-vim/blob/master/.vimrc)
<a name="UOIYv"></a>
### 主题
```bash
syntax enable
set background=dark
colorscheme solarized
```
◈ [altercation/vim-colors-solarized](https://github.com/altercation/vim-colors-solarized)<br />[https://github.com/altercation/vim-colors-solarized](https://github.com/altercation/vim-colors-solarized)<br />◈ [Anthony25/gnome-terminal-colors-solarized](https://github.com/Anthony25/gnome-terminal-colors-solarized)<br />[https://github.com/Anthony25/gnome-terminal-colors-solarized](https://github.com/Anthony25/gnome-terminal-colors-solarized)
<a name="G5nNo"></a>
### 树形目录
```
Plug 'scrooloose/nerdtree'
Plug 'jistr/vim-nerdtree-tabs'
Plug 'Xuyuanp/nerdtree-git-plugin'
autocmd vimenter * NERDTree
map <C-n> :NERDTreeToggle<CR>
let NERDTreeShowHidden=1
let g:NERDTreeShowIgnoredStatus = 1
let g:nerdtree_tabs_open_on_console_startup=1
let g:NERDTreeIndicatorMapCustom = {
    \ "Modified"  : "✹",
    \ "Staged"    : "✚",
    \ "Untracked" : "✭",
    \ "Renamed"   : "➜",
    \ "Unmerged"  : "═",
    \ "Deleted"   : "✖",
    \ "Dirty"     : "✗",
    \ "Clean"     : "✔︎",
    \ 'Ignored'   : '☒',
    \ "Unknown"   : "?"
    \ }
# o 打开关闭文件或目录
# e 以文件管理的方式打开选中的目录
# t 在标签页中打开
# T 在标签页中打开，但光标仍然留在 NERDTree
# r 刷新光标所在的目录
# R 刷新当前根路径
# X 收起所有目录
# p 小写，跳转到光标所在的上一级路径
# P 大写，跳转到当前根路径
# J 到第一个节点
# K 到最后一个节点
# I 显示隐藏文件
# m 显示文件操作菜单
# C 将根路径设置为光标所在的目录
# u 设置上级目录为根路径
# ctrl + w + w 光标自动在左右侧窗口切换
# ctrl + w + r 移动当前窗口的布局位置
# :tabc 关闭当前的 tab
# :tabo   关闭所有其他的 tab
# :tabp   前一个 tab
# :tabn   后一个 tab
# gT      前一个 tab
# gt      后一个 tab
```
◈ [scrooloose/nerdtree](https://github.com/scrooloose/nerdtree)<br />[https://github.com/scrooloose/nerdtree](https://github.com/scrooloose/nerdtree)<br />◈ [vim-nerdtree-tabs](https://github.com/jistr/vim-nerdtree-tabs)<br />[https://github.com/jistr/vim-nerdtree-tabs](https://github.com/jistr/vim-nerdtree-tabs)<br />◈ [nerdtree-git-plugin](https://github.com/Xuyuanp/nerdtree-git-plugin)<br />[https://github.com/Xuyuanp/nerdtree-git-plugin](https://github.com/Xuyuanp/nerdtree-git-plugin)
<a name="WS6rX"></a>
### 代码，引号，路径补全
```
Plug 'Valloric/YouCompleteMe'
Plug 'Raimondi/delimitMate'
Plug 'Shougo/deoplete.nvim', { 'do': ':UpdateRemotePlugins' }
```
◈ [Valloric/YouCompleteMe](https://github.com/Valloric/YouCompleteMe)<br />[https://github.com/Valloric/YouCompleteMe](https://github.com/Valloric/YouCompleteMe)<br />◈ [Raimondi/delimitMate](https://github.com/Raimondi/delimitMate)<br />[https://github.com/Raimondi/delimitMate](https://github.com/Raimondi/delimitMate)<br />◈ [Shougo/deoplete.nvim](https://github.com/Shougo/deoplete.nvim)<br />[https://github.com/Shougo/deoplete.nvim](https://github.com/Shougo/deoplete.nvim)
<a name="f1ff742d"></a>
### 语法高亮，检查
```
Plug 'sheerun/vim-polyglot'
Plug 'w0rp/ale'
let g:ale_linters = {
\    'javascript': ['eslint'],
\    'css': ['stylelint'],
\}
let g:ale_fixers = {
\    'javascript': ['eslint'],
\    'css': ['stylelint'],
\}
let g:ale_fix_on_save = 1
let g:ale_sign_column_always = 1
let g:ale_sign_error = '●'
let g:ale_sign_warning = '▶'
nmap <silent> <C-k> <Plug>(ale_previous_wrap)
nmap <silent> <C-j> <Plug>(ale_next_wrap)
```
◈ [w0rp/ale](https://github.com/w0rp/ale)<br />[https://github.com/w0rp/ale](https://github.com/w0rp/ale)<br />◈ [sheerun/vim-polyglot](https://github.com/sheerun/vim-polyglot)<br />[https://github.com/sheerun/vim-polyglot](https://github.com/sheerun/vim-polyglot)
<a name="Im7NF"></a>
### 文件，代码搜索
```
Plug 'rking/ag.vim'
Plug 'kien/ctrlp.vim'
```
◈ [kien/ctrlp.vim](https://github.com/kien/ctrlp.vim)<br />[https://github.com/kien/ctrlp.vim](https://github.com/kien/ctrlp.vim)<br />◈ [ggreer/the_silver_searcher](https://github.com/ggreer/the_silver_searcher)<br />[https://github.com/ggreer/the_silver_searcher](https://github.com/ggreer/the_silver_searcher)<br />◈ [rking/ag.vim](https://github.com/rking/ag.vim)<br />[https://github.com/rking/ag.vim](https://github.com/rking/ag.vim)
<a name="4200bb79"></a>
### 加强版状态栏
```
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
let g:airline_theme='papercolor'
```
◈ [vim-airline/vim-airline](https://github.com/vim-airline/vim-airline)<br />[https://github.com/vim-airline/vim-airline](https://github.com/vim-airline/vim-airline)<br />◈ [vim-airline/vim-airline-themes](https://github.com/vim-airline/vim-airline-themes)<br />[https://github.com/vim-airline/vim-airline-themes](https://github.com/vim-airline/vim-airline-themes)
<a name="4uxMB"></a>
### 代码注释
```
Plug 'scrooloose/nerdcommenter'
# <leader>cc // 注释
# <leader>cm 只用一组符号注释
# <leader>cA 在行尾添加注释
# <leader>c$ /* 注释 */
# <leader>cs /* 块注释 */
# <leader>cy 注释并复制
# <leader>c<space> 注释/取消注释
# <leader>ca 切换　// 和 /* */
# <leader>cu 取消注释
let g:NERDSpaceDelims = 1
let g:NERDDefaultAlign = 'left'
let g:NERDCustomDelimiters = {
            \ 'javascript': { 'left': '//', 'leftAlt': '/**', 'rightAlt': '*/' },
            \ 'less': { 'left': '/**', 'right': '*/' }
        \ }
```
◈ [scrooloose/nerdcommenter](https://github.com/scrooloose/nerdcommenter)<br />[https://github.com/scrooloose/nerdcommenter](https://github.com/scrooloose/nerdcommenter)
<a name="Sa4sw"></a>
### git
```
Plug 'airblade/vim-gitgutter'
Plug 'tpope/vim-fugitive'
```
◈ [airblade/vim-gitgutter](https://github.com/airblade/vim-gitgutter)<br />[https://github.com/airblade/vim-gitgutter](https://github.com/airblade/vim-gitgutter)<br />◈ [tpope/vim-fugitive](https://github.com/tpope/vim-fugitive)<br />[https://github.com/tpope/vim-fugitive](https://github.com/tpope/vim-fugitive)
<a name="aIeGA"></a>
### Markdown
```
Plug 'suan/vim-instant-markdown'
let g:instant_markdown_slow = 1
let g:instant_markdown_autostart = 0
# :InstantMarkdownPreview
```
◈ [suan/vim-instant-markdown](https://github.com/suan/vim-instant-markdown)<br />[https://github.com/suan/vim-instant-markdown](https://github.com/suan/vim-instant-markdown)
<a name="Emmet"></a>
### Emmet
```
Plug 'mattn/emmet-vim'
let g:user_emmet_leader_key='<Tab>'
let g:user_emmet_settings = {
         \ 'javascript.jsx' : {
            \ 'extends' : 'jsx',
         \ },
      \ }
```
◈ [mattn/emmet-vim](https://github.com/mattn/emmet-vim)<br />[https://github.com/mattn/emmet-vim](https://github.com/mattn/emmet-vim)
<a name="NuqBI"></a>
### html 5
```
Plug'othree/html5.vim'
```
◈ [othree/html5.vim](https://github.com/othree/html5.vim)<br />[https://github.com/othree/html5.vim](https://github.com/othree/html5.vim)
<a name="BNzq3"></a>
### css 3
```javascript
Plug 'hail2u/vim-css3-syntax'
Plug 'ap/vim-css-color'
augroup VimCSS3Syntax
  autocmd!
  autocmd FileType css setlocal iskeyword+=-
augroup END
```
◈ [hail2u/vim-css3-syntax](https://github.com/hail2u/vim-css3-syntax)<br />[https://github.com/hail2u/vim-css3-syntax](https://github.com/hail2u/vim-css3-syntax)<br />◈ [ap/vim-css-color](https://github.com/ap/vim-css-color)<br />[https://github.com/ap/vim-css-color](https://github.com/ap/vim-css-color)
<a name="IzeO8"></a>
### JavaScipt
```javascript
Plug 'pangloss/vim-javascript'
let g:javascript_plugin_jsdoc = 1
let g:javascript_plugin_ngdoc = 1
let g:javascript_plugin_flow = 1
set foldmethod=syntax
let g:javascript_conceal_function             = "ƒ"
let g:javascript_conceal_null                 = "ø"
let g:javascript_conceal_this                 = "@"
let g:javascript_conceal_return               = "⇚"
let g:javascript_conceal_undefined            = "¿"
let g:javascript_conceal_NaN                  = "ℕ"
let g:javascript_conceal_prototype            = "¶"
let g:javascript_conceal_static               = "•"
let g:javascript_conceal_super                = "Ω"
let g:javascript_conceal_arrow_function       = "⇒"
let g:javascript_conceal_noarg_arrow_function = " "
let g:javascript_conceal_underscore_arrow_function = " "
set conceallevel=1
```
◈ [pangloss/vim-javascript](https://github.com/pangloss/vim-javascript)<br />[https://github.com/pangloss/vim-javascript](https://github.com/pangloss/vim-javascript)
:::warning
（注：上述脚本中存在特殊字符，有的情况下显示不正确，请直接用上述链接的内容。）
:::
<a name="7Ctz5"></a>
### React
```javascript
Plug 'mxw/vim-jsx'
let g:jsx_ext_required = 0
```
◈ [mxw/vim-jsx](https://github.com/mxw/vim-jsx)<br />[https://github.com/mxw/vim-jsx](https://github.com/mxw/vim-jsx)
<a name="Prettier"></a>
### Prettier
```javascript
Plug 'prettier/vim-prettier', {
  \ 'do': 'yarn install',
  \ 'for': ['javascript', 'typescript', 'css', 'less', 'scss', 'json', 'graphql'] }
let g:prettier#config#bracket_spacing = 'true'
let g:prettier#config#jsx_bracket_same_line = 'false'
let g:prettier#autoformat = 0
autocmd BufWritePre *.js,*.jsx,*.mjs,*.ts,*.tsx,*.css,*.less,*.scss,*.json,*.graphql PrettierAsync
# :Prettier
```
◈ [prettier/vim-prettier](https://github.com/prettier/vim-prettier)<br />[https://github.com/prettier/vim-prettier](https://github.com/prettier/vim-prettier)
<a name="cBawB"></a>
## 其他插件
<a name="D3gvS"></a>
### 1. Volt
![](https://cdn.nlark.com/yuque/0/2020/gif/396745/1596072977733-f3b4bd32-030c-4821-bcb7-4c7dccff4d14.gif#align=left&display=inline&height=261&originHeight=261&originWidth=639&size=0&status=done&style=none&width=639)<br />Volt是一个插件管理器，可以通过它来安装插件并创建名为“配置文件”的插件组合，Volt仍然相对较新，所以还不够完美（例如，每个插件只能有一个配置文件），但除此之外，它非常方便、非常快、非常简单。
<a name="2tZsf"></a>
### 2. Vim-Rainbow
![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1596072977618-5520b90c-717a-4669-af9d-467339e86d4e.webp#align=left&display=inline&height=158&originHeight=158&originWidth=931&size=0&status=done&style=none&width=931)<br />几乎所有主流的编程语言都需要用到方括号，圆括号，花括号等，通常，会使用多对的括号，要弄清楚哪个右括号属于哪个左括号可能会变得困难和烦人。而vim-rainbow插件会给每一对括号加一个独特的颜色，这样就很容易辨认了，而且它不仅很有用，也会让代码变得丰富多彩
<a name="7eF7O"></a>
### 3. lightline
![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1596072977579-4efd7b9b-6692-4095-be7a-4d93dbdc3e6b.webp#align=left&display=inline&height=197&originHeight=197&originWidth=950&size=0&status=done&style=none&width=950)<br />Vim的插件有很多，比如Powerline，它在屏幕的底部放一个栏，说明当前在处理什么文件，在文件的什么位置，它是什么类型的文件等等，每个插件都有优点和缺点。它相对较小，易于设置，它具有相当的可扩展性，并且不需要任何其他工具或插件。
<a name="pYuqk"></a>
### 4. NERDTree
![](https://cdn.nlark.com/yuque/0/2020/gif/396745/1596072977619-6ec8bf1a-a1c3-43e8-8d0b-35f8827c3ec3.gif#align=left&display=inline&height=248&originHeight=248&originWidth=639&size=0&status=done&style=none&width=639)<br />该插件显示一个目录树，可以执行文件的操作命令。在大型项目中，很难找到要编辑的那一行的文件确切名称和位置。通过快捷键，一个资源管理器窗口就会以目录树的方式打开，可以轻松找到想要的文件并打开它，假如有大量代码或者容易忘记文件名的人，NERDTree是必须要有的
<a name="xLJDv"></a>
### 5. NERD Commenter
![](https://cdn.nlark.com/yuque/0/2020/gif/396745/1596072977588-b9ac152c-b8f7-4496-9c35-b173b52f241f.gif#align=left&display=inline&height=204&originHeight=204&originWidth=639&size=0&status=done&style=none&width=639)<br />有时，编写的代码都会产生难以调试的问题，导致决定注释掉或撤消代码，NERD Commenter的用处就来了，选择代码，点击Leader+cc，代码就会被注释掉；点击Leader+cn，代码将被取消注释。对于大多数文件类型，NERD Commenter都能使用正确的注释字符
<a name="aaQW9"></a>
### 6. Solarized
Solarized 是目前最完整的 Terminal/Editor/IDE 配色项目。<br />通过Solarized，可以选择任何喜欢的颜色方案，此外，Volt插件管理可以轻松地在两者之间进行切换，因此可以使用Monokai进行Python编程，使用编写Bash用Sola。
<a name="ytz3V"></a>
### 7. fzf
![](https://cdn.nlark.com/yuque/0/2020/gif/396745/1596072977619-83e32661-0792-44e1-beee-d741102a01e8.gif#align=left&display=inline&height=204&originHeight=204&originWidth=639&size=0&status=done&style=none&width=639)<br />fzf是目前最快的fuzzy finder（模糊搜索神器），它适用于Fedora、Debian和Arch，不过它有个小缺点是在fzf二进制文件中有一个外部依赖项，所以需要安装缺少的外部依赖。
<a name="NcpUF"></a>
### 8. ack
![](https://cdn.nlark.com/yuque/0/2020/gif/396745/1596072977700-a52bf86e-f487-4ff5-a542-0c335edfa37f.gif#align=left&display=inline&height=346&originHeight=346&originWidth=639&size=0&status=done&style=none&width=639)<br />当需要搜索包含某一行或者某个单词的文件时，最好将它和ag结合使用，这是一个名为“The Silver Searcher”的全局搜索工具。这个组合非常快，涵盖了grep或vimgrep做的绝大多数事情。<br />缺点是需要安装ack或ag才能正常运行，<br />优点是ag和ack都可以用于Fedora和EPEL7。
<a name="LIlDM"></a>
### 9. gitgutter
![](https://cdn.nlark.com/yuque/0/2020/webp/396745/1596072977626-1b925cb4-a82f-42cc-9c05-ce3fd1f6a38d.webp#align=left&display=inline&height=288&originHeight=288&originWidth=954&size=0&status=done&style=none&width=954)<br />使用Git和Git存储库中的文件。该插件支持在状态行中嵌入已添加/已删除/已修改行数的功能。对应的符号如下：更改(~)、添加(+)和删除(-)行。这对于跟踪所更改的内容非常有用，而且还能专注于手头的工作，不过，这个插件有个小小的性能缺陷，有时它的反应速度有点慢，不过依然很有用
<a name="mgtnz"></a>
### 10. Tag List
如果在一个文件中进行编程，那很容易就忘记所处的位置，可能需要自己手动上下滚动查找某个函数，有了Tag List插件，只需输入:Tlist，就可以得到一个垂直分割，其中包含可以轻松跳转到的变量、类型、类和函数。它适用于许多语言，比如Java、Python，以及ctags工具可以处理的任何其他文件类型
